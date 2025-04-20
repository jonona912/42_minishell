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
        echo -e "\e[35mMinishell output: \n \e[0m'$output_minishell'"
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

# Tests
test_command "ls -a" \
            "yes"
test_command "ls" \
            "yes"
test_command "<input.txt       cat" \
            "no"
test_command "<input.txt	cat | grep jnn" \
            "no"
test_command "<input.txt	cat | grep jnn | wc -l" \
            "no"
test_command "(ls -a | grep mini) || (ls -l | grep mini)" \
            "no"
test_command "grep jnn < input.txt < input_2.txt" \
            "no"



################### DOUBLE QUOTES TESTS ########################
# grep jnn < "input.txt" < "input_2.txt"
echo -e "\e[33mDOUBLE QUOTES TESTS\e[0m"
test_command "\cat \"input.txt\"" \
            "no"
test_command "grep jnn < \"input.txt\" < \"input_2.txt\"" \
            "no"






# test_command "(ls -a | grep mini) && (ls | grep mini)" "(ls -a | grep mini) && (ls | grep mini)" "no"
# test_command "(ls -a | grep mini) > output.txt && (ls -a | grep minishell_sub)" "(ls -a | grep mini) > output.txt && (ls -a | grep minishell_sub)" "yes"





