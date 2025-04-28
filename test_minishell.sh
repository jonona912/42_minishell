#!/bin/bash
# compare_outputs() {
#     # Local Variables: The local keyword ensures test_name, output_real, and output_minishell
# 	# are scoped to the function, preventing conflicts with global variables.
# 	local test_name=$1
#     local output_real=$2
#     local output_minishell=$3
# 	# Tests if the two outputs are identical (string equality).
#     if [ "$output_real" = "$output_minishell" ]; then
#         echo -e "\e[32m${test_name} test passed\e[0m"
#     else
#         echo -e "\e[31m${test_name} test failed\e[0m"
#         echo -e "\e[34mReal shell output (sorted):\n \e[0m'$output_real'"
#   		echo -e "\e[35mMinishell output (sorted): \n \e[0m'$output_minishell'"
#     fi
# }

# # Test 1: ls
# echo -n "Testing ls..."
# output_real=$(ls -a| tr ' ' '\n' | sort | tr '\n' ' ' | sed 's/ $//')  # Normalize and sort
# output_minishell=$(echo "ls -a" | ./minishell | tr ' ' '\n' | sort | tr '\n' ' ' | sed 's/ $//')  # Normalize and sort
# compare_outputs "ls" "$output_real" "$output_minishell"


#!/bin/bash

# Function to compare real shell and minishell outputs
compare_outputs() {
    local test_name=$1
    local output_real=$2
    local output_minishell=$3

    if [ "$output_real" = "$output_minishell" ]; then
        echo -e "\e[32m${test_name} test passed\e[0m"
    else
        echo -e "\e[31m${test_name} test failed\e[0m"
        echo -e "\e[34mReal shell output: \n \e[0m'$output_real'"
        echo -e "\e[33mMinishell output: \n \e[0m'$output_minishell'"
    fi
}

# Function to test a command, with optional sorting/normalization
test_command() {
    # local test_name=$1
    local command=$1
    # local command=$3
    local normalize=$2

    # echo -n "Testing ${test_name}..."

    if [ "$normalize" = "yes" ]; then
        output_real=$(eval "$command" | tr ' ' '\n' | sort | tr '\n' ' ' | sed 's/ $//')
        output_minishell=$(echo "$command" | ./minishell -test | tr ' ' '\n' | sort | tr '\n' ' ' | sed 's/ $//')
    else
        output_real=$(eval "$command")
        output_minishell=$(echo "$command" | ./minishell -test)
    fi
    compare_outputs "$command" "$output_real" "$output_minishell"
}

# invalid permission tests
# Tests
echo -e "\e[35mNO ARGUMENTS TEST\e[0m" #
test_command "" \
            "no"

echo -e "\e[35mSIMPLE TESTS\e[0m" #

test_command "ls -a" \
            "yes"
test_command "ls" \
            "yes"

echo -e "\e[35mPIPE TESTS\e[0m" #
test_command "ls -a | grep mini" \
            "no"

test_command "ls -a | grep mini | wc -l" \
            "no"

echo -e "\e[35mREDIRECTION IN TESTS\e[0m" #
test_command "<input.txt       cat" \
            "no"
test_command "<input.txt	cat | grep jnn" \
            "no"
test_command "<input.txt	cat | grep jnn | wc -l" \
            "no"
test_command "grep jnn < input.txt < input_2.txt" \
            "no"

echo -e "\e[35mAND / OR TESTS (&&/||) \e[0m" #
test_command "(ls -a | grep mini) || (ls -l | grep mini)" \
            "no"

test_command "cat input.txt || ls -a" \
            "no"

test_command "cat input.txt && cat input_2.txt" \
            "no"

test_command "cat input.txt | wc -l && cat input_2.txt" \
            "no"

test_command "< input.txt cat | wc -l && cat input_2.txt" \
            "no"

echo -e "\e[35mSUBSHELL TESTS\e[0m" #

test_command "(ls -a | grep mini | wc -l) && (ls -l | grep w | wc -l)" \
            "no"

test_command "(ls -a && cat < input.txt)" \
            "yes"

test_command "((ls -a| grep mini) || cat < input.txt)" \
            "no"

test_command "(ls -a && cat < input.txt) | (grep .txt | (grep input | wc -l))" \
            "no"

test_command "(ls -a| sort && cat < input.txt) && (((ls -a && cat < input.txt) | grep .txt) | (grep input | wc -l))" \
            "yes"



################## DOUBLE QUOTES TESTS ########################
echo -e "\e[33mDOUBLE QUOTES TESTS\e[0m"
test_command "cat \"input.txt\"" \
            "no"

test_command "grep jnn < \"input.txt\" < \"input_2.txt\"" \
            "no"


################## SIGNLE QUOTES TESTS ########################
echo -e "\e[33mSINGLE QUOTES TESTS\e[0m"
test_command "cat 'input.txt'" \
            "no"
test_command "grep jnn < 'input.txt' < 'input_2.txt'" \
            "no"


################## BUILTINS TESTS ########################
echo -e "\e[33m===========> BUILTINS TESTS <============\e[0m"

echo -e "\e[33mECHO TESTS\e[0m"

test_command "echo hello jnn" \
            "no"

test_command "echo \"hello \$USER\"" \
            "no"

test_command "echo -n \"hello \$PATH\"" \
            "no"

test_command "echo -n \"hello jnn\"" \
            "no"

test_command "echo -nnnnnn \"hello jnn\"" \
            "no"

test_command "echo -n -n -n -n -n -n -n -n \"hello jnn\"" \
            "no"

test_command "echo -n -n -n -n -n -n -n -n -n\"hello jnn\"" \
            "no"

test_command "echo -n -n -n -n -n -n -n -n \"hello jnn\" | wc -l" \
            "no"

test_command "echo -n -n hello jnn, there should be seven words | wc -w" \
            "no"

test_command "echo -n 'hello \$USER'" \
            "no"

test_command "echo \$USER \"\$USER'\$USER'\"'\$USER\"\$USER\"'"

test_command "echo \$NON_AVAL" \
            "no"



test_command "echo \$? + \$? - \$?" \
            "no"
# add more single quote and other types of tests

echo -e "\e[33mPWD TESTS\e[0m"

test_command "pwd" \
            "no"

test_command "pwd | grep home" \
            "no"

test_command "pwd | grep home | wc -l" \
            "no"


# echo -e "\e[33m===========> WILDCARD TESTS <============\e[0m"
echo -e "\e[33mWILDCARD TESTS\e[0m"

test_command "echo *" \
            "yes"

test_command "echo *****" \
            "yes"

test_command "echo *.txt" \
            "yes"

test_command "echo ******.txt" \
            "yes"

test_command "echo *.txt | wc -l" \
            "no"

test_command "echo *.txt | wc -l | grep 2" \
            "yes"

test_command "echo mini*" \
            "yes"

test_command "echo mini*******" \
            "yes"

test_command "echo mini***** | wc -l" \
            "no"

test_command "cat input*" \
            "no"

test_command "cat input* | wc -l" \
            "no"

test_command "cat *.txt | wc -l" \
            "no"

##############################
### BASIC COMMAND TESTS
##############################
echo -e "\n\e[35m=== BASIC COMMAND TESTS ===\e[0m"
test_command "ls" "yes"
test_command "ls -la" "yes"
test_command "/bin/ls" "yes"
test_command "/bin/ls -la" "yes"
test_command "echo Hello World" "no"
test_command "echo -n Hello World" "no"

##############################
### REDIRECTION TESTS
##############################
echo -e "\n\e[35m=== REDIRECTION TESTS ===\e[0m"

# Input redirection
test_command "cat < test_input.txt" "no"
test_command "wc -l < test_input.txt" "no"

# Output redirection
test_command "echo new content > test_output.txt" "no"
test_command "cat test_output.txt" "no"  # Should show "new content"

# Append redirection
test_command "echo appended content >> test_append.txt" "no"
test_command "cat test_append.txt" "no"  # Should show original + appended content


##############################
### PIPE TESTS
##############################
echo -e "\n\e[35m=== PIPE TESTS ===\e[0m"
test_command "ls | grep Makefile" "no"
test_command "ls | grep Makefile | wc -l" "no"
test_command "cat test_input.txt | grep test" "no"
test_command "cat test_input.txt | grep test | wc -c" "no"

##############################
### ENVIRONMENT VARIABLE TESTS
##############################
echo -e "\n\e[35m=== ENVIRONMENT VARIABLE TESTS ===\e[0m"
test_command "echo \$PATH" "no"
test_command "echo \"\$PATH\"" "no"
test_command "echo '\$PATH'" "no"
test_command "echo \$NON_EXISTENT_VAR" "no"
test_command "echo \"\$NON_EXISTENT_VAR\"" "no"
test_command "echo '\$NON_EXISTENT_VAR'" "no"

##############################
### QUOTING TESTS
##############################
echo -e "\n\e[35m=== QUOTING TESTS ===\e[0m"
test_command "echo \"hello \$USER\"" "no"
test_command "echo 'hello \$USER'" "no"
test_command "echo \"hello '\$USER'\"" "no"
test_command "echo 'hello \"\$USER\"'" "no"
test_command "echo 'hello \\"world\\"'" "no"

##############################
### WILDCARD TESTS
##############################
echo -e "\n\e[35m=== WILDCARD TESTS ===\e[0m"
test_command "echo *.txt" "yes"
test_command "echo *" "yes"
test_command "echo *.nonexistent" "yes"
test_command "echo * | wc -w" "no"


# exit status is same as in bash for the folowing cases
# $? returns correct value for <invalid command>, e.g. wrong_command
# <valid command> and/or <valid flags> e.g. ls -asdf, ls, ls -a
# returns correct value for <valid command> and <invalid flags> e.g.  ls -resa
# cases to handle echo $?. echo $? + $?. echo $? + $? a sentence $? ...
# Below are examples how this should work in bash
# mnishel: $? + $?
# 0: command not found
# mnishel: alsdjf
# alsdjf: command not found
# mnishel: $? + $?
# 127: command not found
# mnishel: ls -rere
# ls: invalid option -- 'e'
# Try 'ls --help' for more information.
# mnishel: $? + $?
# 2: command not found
# mnishel: ls -asdf
# .
# mnishel: $? + $?
# 0: command not found
# mnishel:




# ======================================================>   problems
# cat << EOF
# cat << EOF > output.txt
# cat << EOF > output.txt | cat << END > output2.txt

# fasdfasdf || ls
# fasldjkfasd && ls
# ls -a && ls -l => works

#  % ./minishell
# minishel> << EOF
# zsh: segmentation fault (core dumped)  ./minishell

# minishel> ls o**.txt
# ls: cannot access '.txt': No such file or directory
# output  output.txt



############################### opopov's territory
# Should print exit after command
# opopov@c2r13s4:~/Documents/Minishell/minishell$ exit dfgdfg
# exit
# bash: exit: dfgdfg: numeric argument required


# Solve problem with CTRL+C and cat
# % ./minishell
# minishel> cat^C
# minishel> cat
# minishel> minishel> fdgdfg

# Solve problem with CTRL+D
# % ./minishell
# minishel> cat
# ^\^\^\minishel>

# Problem with unset

# Problem with export
