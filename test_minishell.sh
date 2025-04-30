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

# Making folders

# invalid permission tests
# Tests
####################=================================================================================================
# echo -e "\e[35mNO ARGUMENTS TEST\e[0m" #
# test_command "" \
#             "no"

# echo -e "\e[35mSIMPLE TESTS\e[0m" #

# test_command "ls -a" \
#             "yes"
# test_command "ls" \
#             "yes"

# echo -e "\e[35mPIPE TESTS\e[0m" #
# test_command "ls -a | grep mini" \
#             "no"

# test_command "ls -a | grep mini | wc -l" \
#             "no"

# echo -e "\e[35mREDIRECTION IN TESTS\e[0m" #
# test_command "<input.txt       cat" \
#             "no"
# test_command "<input.txt	cat | grep jnn" \
#             "no"
# test_command "<input.txt	cat | grep jnn | wc -l" \
#             "no"
# test_command "grep jnn < input.txt < input_2.txt" \
#             "no"

# echo -e "\e[35mAND / OR TESTS (&&/||) \e[0m" #
# test_command "(ls -a | grep mini) || (ls -l | grep mini)" \
#             "no"

# test_command "cat input.txt || ls -a" \
#             "no"

# test_command "cat input.txt && cat input_2.txt" \
#             "no"

# test_command "cat input.txt | wc -l && cat input_2.txt" \
#             "no"

# test_command "< input.txt cat | wc -l && cat input_2.txt" \
#             "no"

# echo -e "\e[35mSUBSHELL TESTS\e[0m" #

# test_command "(ls -a | grep mini | wc -l) && (ls -l | grep w | wc -l)" \
#             "no"

# test_command "(ls -a && cat < input.txt)" \
#             "yes"

# test_command "((ls -a| grep mini) || cat < input.txt)" \
#             "no"

# test_command "(ls -a && cat < input.txt) | (grep .txt | (grep input | wc -l))" \
#             "no"

# test_command "(ls -a| sort && cat < input.txt) && (((ls -a && cat < input.txt) | grep .txt) | (grep input | wc -l))" \
#             "yes"

####################=================================================================================================

################## DOUBLE QUOTES TESTS ########################
echo -e "\e[33mDOUBLE QUOTES TESTS\e[0m"
test_command "cat \"input.txt\"" \
            "no"
# grep jnn < "input.txt" < "input_2.txt"
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
##########################################################=====================================
# echo -e "\e[33mPWD TESTS\e[0m"

# test_command "pwd" \
#             "no"

# test_command "pwd | grep home" \
#             "no"

# test_command "pwd | grep home | wc -l" \
#             "no"


# # echo -e "\e[33m===========> WILDCARD TESTS <============\e[0m"
# echo -e "\e[33mWILDCARD TESTS\e[0m"

# test_command "echo *" \
#             "yes"

# test_command "echo *****" \
#             "yes"

# test_command "echo *.txt" \
#             "yes"

# test_command "echo ******.txt" \
#             "yes"

# test_command "echo *.txt | wc -l" \
#             "no"

# test_command "echo *.txt | wc -l | grep 2" \
#             "yes"

# test_command "echo mini*" \
#             "yes"

# test_command "echo mini*******" \
#             "yes"

# test_command "echo mini***** | wc -l" \
#             "no"

# test_command "cat input*" \
#             "no"

# test_command "cat input* | wc -l" \
#             "no"

# test_command "cat *.txt | wc -l" \
#             "no"

# ##############################
# ### BASIC COMMAND TESTS
# ##############################
# echo -e "\n\e[35m=== BASIC COMMAND TESTS ===\e[0m"
# test_command "ls" "yes"
# test_command "ls -la" "yes"
# test_command "/bin/ls" "yes"
# test_command "/bin/ls -la" "yes"
# test_command "echo Hello World" "no"
# test_command "echo -n Hello World" "no"

# ##############################
# ### REDIRECTION TESTS
# ##############################
# echo -e "\n\e[35m=== REDIRECTION TESTS ===\e[0m"

# # Input redirection
# test_command "cat < input.txt" "no"
# test_command "wc -l < input.txt" "no"

# # Output redirection
# test_command "echo new content > test_output.txt" "no"
# test_command "cat test_output.txt" "no"  # Should show "new content"

# # Append redirection
# test_command "echo appended content >> test_append.txt" "no"
# test_command "cat test_append.txt" "no"  # Should show original + appended content


# ##############################
# ### PIPE TESTS
# ##############################
# echo -e "\n\e[35m=== PIPE TESTS ===\e[0m"
# test_command "ls | grep Makefile" "no"
# test_command "ls | grep Makefile | wc -l" "no"
# test_command "cat input.txt | grep test" "no"
# test_command "cat input.txt | grep test | wc -c" "no"

# ##############################
# ### ENVIRONMENT VARIABLE TESTS
# ##############################
# echo -e "\n\e[35m=== ENVIRONMENT VARIABLE TESTS ===\e[0m"
# test_command "echo \$PATH" "no"
# test_command "echo \"\$PATH\"" "no"
# test_command "echo '\$PATH'" "no"
# test_command "echo \$NON_EXISTENT_VAR" "no"
# test_command "echo \"\$NON_EXISTENT_VAR\"" "no"
# test_command "echo '\$NON_EXISTENT_VAR'" "no"

# ##############################
# ### QUOTING TESTS
# ##############################
# echo -e "\n\e[35m=== QUOTING TESTS ===\e[0m"
# test_command "echo \"hello \$USER\"" "no"
# test_command "echo 'hello \$USER'" "no"
# test_command "echo \"hello '\$USER'\"" "no"
# test_command "echo 'hello \"\$USER\"'" "no"
# test_command "echo 'hello \\"world\\"'" "no"

# ##############################
# ### WILDCARD TESTS
# ##############################
# echo -e "\n\e[35m=== WILDCARD TESTS ===\e[0m"
# test_command "echo *.txt" "yes"
# test_command "echo *" "yes"
# test_command "echo *.nonexistent" "yes"
# test_command "echo * | wc -w" "no"

##########################################################33===============================================
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

# fasdfasdf || ls
# fasldjkfasd && ls
# ls -a && ls -l => works

#  % ./minishell
# minishel> << EOF
# zsh: segmentation fault (core dumped)  ./minishell

# (ls -a | grep mini > output.txt) || (ls -l | grep mini > output.txt)1


############################### opopov's territory

# Solve problem with CTRL+C and cat
# % ./minishell
# minishel> cat^C
# minishel> cat
# minishel> minishel> fdgdfg


# minishel> echo "somme test" >>>> testfile1.txt
# minishell: Redirection without or wrong target
# minishell: syntax error near unexpected token


# minishel> hgfdgfd
# hgfdgfd: No such file or directory
# minishel> hgfdgfd
# hgfdgfd: No such file or directory
# minishel> echo "Start" & echo "middle"
# ^C
# minishel> echo start
# ^C
# minishel> ^[[A^[[A^[[B^[[B^[[A^[[A^[[A







# new issues:
#### Ctrl+C doesn't work in heredoc (Maybe need to work on this together)
# ^Cminishell> 
# heredoc>
# minishell> ^C
# heredoc>
# minishell> ^C
# heredoc>
# minishell> ^C


# exit should exit with the specified number e.g. exit 12 -> exit status 12, 
# exit 12 292 393 (error too many arguments...) exit asdfasdf (different exit status, probably 2)

# export display should be different from env
# echo 'exit_code ->$? user ->$USER home -> $HOME'

# execution of PATH (Unsetting Path should disable the execution of ls) -----> DONE


# echo 'exit_code ->$? user ->$USER home -> $HOME' - Does not behave same as bash
# wrong exit status for this:  export A-, export =      
# this should fail:  export HELLO-=123
#  cd $PWD hi (similar to exit)
# echo hi | > outfile01 echo bye >invalid_permission   ===> DONE
