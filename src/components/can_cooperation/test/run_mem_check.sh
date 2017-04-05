#!/bin/bash
# Srcipt to call valgring check

OPTIND=1
only_install=0
install_quietly=0
command_name="valgrind"

show_help() {
  echo "
        Usage: ./run_mem_check.sh [OPTION]...
          Runs valgring against binary.
        Arguments:
          -h    displays help message and exits
          -b    specifies binary to be tested. Default is can_cooperation_test.
          -q    install missing software without prompting. Default is false.
          -i    only install missing software and exit. No prompts will be issued.
          -x    <file> save XML output to file. Only for memcheck tool.
          -t    <name> module tool name for analyse (memcheck - default, callgrind, massif and etc.)
          -f    <filter> to run selected tests (see examples below)

        GUI tools which you can use to analyse:
          Valkyrie - memcheck
          KCachegrind- callgrind
          Massif-Visualizer - massif

        Example:
          ./run_mem_check.sh -b ./test_can_module
          ./run_mem_check.sh -t callgrind
          ./run_mem_check.sh -x output.xml
          ./run_mem_check.sh -f CanTcpConnectionTest*
          ./run_mem_check.sh -f *WriteData
"
}

installation_prompt() {
  read -n 1 -p "Would you like to install $command_name? [Y/n] " is_ok
  echo
  if [ "$is_ok" == "y" -o "$is_ok" == "Y" ]
  then
    sudo apt-get install $command_name
  else
    echo "$command_name will no be installed. Quiting."
    exit 0
  fi
}

install_if_not() {
  if command -v $command_name >/dev/null 2>&1; then
    echo "Run $command_name"
  else 
    echo "Installation of $command_name is required."
    if [ $install_quietly -eq 1 ]; then
      sudo apt-get install $command_name
    else
      installation_prompt     
    fi
  fi
}

binary_test="./can_cooperation_test"

tool_name=memcheck
while getopts "t:x:hqib:f:" opt; do
    case "$opt" in
    h)
        show_help
        exit 0
        ;;
    q)  install_quietly=1
        ;;
    i)  only_install=1
        install_quietly=1
        install_if_not
        exit 0
        ;;
    b)  binary_test=$OPTARG
        ;;
    x)  xml_report="--xml=yes --xml-file=$OPTARG"
        ;;
    t)  tool_name=$OPTARG
        ;;
    f)  gtest_filter="--gtest_filter=$OPTARG"
        ;;
    esac
done

install_if_not

if [ "$tool_name" == "memcheck" ]; then
    params="--leak-check=full --leak-check-heuristics=all --show-leak-kinds=all"
fi

$command_name --tool=$tool_name $params $xml_report $binary_test $gtest_filter
