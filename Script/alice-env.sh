#!/bin/bash

# colors
Cm="\033[35m"
Cy="\033[33m"
Cc="\033[36m"
Cb="\033[34m"
Cg="\033[32m"
Cr="\033[31m"
Cw="\033[37m"
Cz="\033[m"
Bo="\033[1m"
Br="\033[41m"
By="\033[43m"

#
# Functions
#

# interactively pick the ALICE software tuple: returns nothing, result is a number stored in the
# nAliTuple variable
function AliMenu() {

  local idx

  # header
  echo
  echo -e "${Cm}Select an ALICE software tuple from below.${Cz}"
  echo
  echo -e "${Cm}Note: ${Cc}you might as well source this script with \"-m <query>\" for a"
  echo -e "      non-interactive selection or with \"-c\" to clean the envrionment.${Cz}"
  echo

  # list of tuples
  for (( idx=1 ; idx<=${#AliTuple[@]} ; idx++ )) ; do
    printf " ${Cc}% 2d.${Cz}" $idx
    AliParseModVer ${AliTuple[$idx]} m v
    if [[ $m == "AliPhysics" ]] ; then
      echo -e " ${Cg}$m${Cz} ${v/latest-/}"
    else
      echo -e " ${Cb}$m${Cz} ${v/latest-/}"
    fi
    unset m v
  done
  
  # option to clean
  echo
  echo -e " ${Cc} 0.${Cz} Clean environment"

  # prompt
  while [[ 1 ]] ; do
    echo
    echo -ne 'Your choice (type a number and press ENTER): '
    read nAliTuple
    if [[ ! $nAliTuple =~ ^[[:digit:]]+$ ]] ; then
      echo -e "${Cr}Not a number${Cz}"
    elif [[ $nAliTuple -gt ${#AliTuple[@]} ]] ; then
      echo -e "${Cr}Out of range${Cz}"
    else
      break
    fi
  done

  # "return" variable
  export nAliTuple
  return 0

}

# separate the module name and version
function AliParseModVer() {
  local modVer="${1##*\@}"
  local modVar="$2"
  local verVar="$3"
  eval "$modVar='${modVer%%:*}' ; $verVar='${modVer##*:}'"
}

# finds tuple index by name
function AliTupleNumberByQuery() (

  local tuple_matches=0
  local count_tuple=0
  local tup
  
  for tup in "${AliTuple[@]}" ; do
  
    count_tuple=$(( count_tuple + 1 ))
    
    AliParseModVer $tup m v
    if [[ $1 == ${v/latest-/} ]] ; then
      tuple_matches=1
    fi
    unset m v
    
    if [[ $tuple_matches == 1 ]] ; then
      # matching tuple found
      echo $count_tuple
      return 0
    fi

  done

  # no matching tuple found: echo nothing
  return 1
  
)

# load aliroot/aliphysics environment
function AliLoad() {
  if [[ $1 -gt 0 ]] ; then
    AliParseModVer ${AliTuple[$1]} m v
    alienv load $m/$v
    export ALICE_LOADED_MODULE="[$m/${v/latest-/}]"
    export PS1='\033[34m$ALICE_LOADED_MODULE\033[m'$ORIGINAL_PS1
    export ALICE_ENV_DONT_CHANGE_PS1=1
    unset m v
  fi
}

# unload aliroot/aliphysics environment
function AliUnload() {
  while [[ 1 ]] ; do
    local loadedMod=`alienv list 2>&1 | grep -v Modulefiles | sed 's/) /)/g' | xargs -n1 | sort -n | tail -n1 | cut -d ")" -f2`
    if [ -z $loadedMod ]; then
      break
    else
      alienv -q unload $loadedMod
    fi
  done
  export ALICE_LOADED_MODULE=""
}

# main function: takes parameters from the command line
function AliMain() {

  local OPT_NONINTERACTIVE=0
  local OPT_CLEANENV=0
  local queryAliTuple

  # parse command line options
  while [[ $# -gt 0 ]] ; do
    case "$1" in
      -m)
        OPT_NONINTERACTIVE=1
        queryAliTuple="$2"
        shift
      ;;
      -c) OPT_CLEANENV=1; ;;
    esac
    shift
  done

  # stop here when cleaning environment
  if [[ "$OPT_CLEANENV" == 1 ]] ; then
    AliUnload
    echo -e "${Cy}ALICE environment variables purged${Cz}"
    return 0
  fi

  # build the list of available aliroot/aliphysics versions
  idx=1
  unset AliTuple
  for tup in `alienv q | grep latest-` ; do
    AliTuple[$idx]=$tup
    idx=$(( idx + 1 ))
  done

  if [[ "$OPT_NONINTERACTIVE" != 1 ]] ; then
  
    # print menu if non-interactive
    AliMenu
    
  elif [[ $queryAliTuple != '' ]] ; then
  
    # selection by query (-m <query>)
    nAliTuple=$( AliTupleNumberByQuery "$queryAliTuple" )
    if [[ ! $nAliTuple =~ ^[[:digit:]]+$ || $nAliTuple -gt ${#AliTuple[@]} ]] ; then
      echo -e "${Cr}No tuple matches the given query: ${Cb}${queryAliTuple}${Cz}"
      return 1
    fi
    
  else
  
    echo -e "${Cy}invalid option${Cz}"
    
  fi

  # cleans up the environment from previous variables
  AliUnload
  [[ $nAliTuple == 0 ]] && echo -e "${Cy}ALICE environment variables purged${Cz}"
  
  # load new environment
  AliLoad $nAliTuple
  return 0

}

#
# Entry point
#

AliMain "$@"
ALI_rv=$?
return $ALI_rv
