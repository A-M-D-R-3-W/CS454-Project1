// PROJECT 1

// TEAM: ANDREW GARBEROLIO AND JOHN MACINTYRE

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <numeric>
#include <cmath>
#include <boost/multiprecision/cpp_int.hpp>

using Big = boost::multiprecision::cpp_int;

const int ALPHABET_SIZE = 4;
const int MEMORY_LENGTH= 5; // Last 5 chars in DFA 

const std::vector<char> ALPHABET = {'a', 'b', 'c', 'd'};

const int NUM_STATES = 1365;
const int FAIL_STATE = 1365;
const int NUM_TOTAL_STATES = 1366;


int stringToState(const std::string& buffer) {
    if (buffer.empty()) {
        return 0;
    }

    int offset = 0;

    for (int i = 1; i < buffer.length(); i++) {
        offset += static_cast<int>(pow(ALPHABET_SIZE, i));
    }
    offset += 1;

    int val = 0;
    for (char c : buffer) {
        val = val * ALPHABET_SIZE + (c - 'a');
    }

    return offset + val;
}

std::string stateToString(int state_id) {
    if (state_id == 0) {
        return "";
    }

    int len = 1;
    int states_at_len = ALPHABET_SIZE;
    int prev_states = 1;

    while (state_id >= prev_states + states_at_len) {
        prev_states += states_at_len;
        len++;
        states_at_len *= ALPHABET_SIZE;
    }

    int val = state_id - prev_states;

    std::string s(len, 'a');

    for (int i = len - 1; i >= 0; i--) {
        s[i] = static_cast<char>('a' + (val % ALPHABET_SIZE));
        val /= ALPHABET_SIZE;
    }
    return s;
}

// Builds a transition table for the DFA
std::vector<std::vector<int>> dfa_builder() {
    std::vector<std::vector<int>> dfa(NUM_TOTAL_STATES, std::vector<int>(ALPHABET_SIZE));

    for (int state = 0; state < NUM_STATES; state++) {
        std::string buffer = stateToString(state);

        for (int char_id = 0; char_id < ALPHABET_SIZE; char_id++) {
            char next_char = 'a' + char_id;
            std::string next_buffer = buffer + next_char;
            int next_state;

            if (next_buffer.length() < MEMORY_LENGTH + 1) {
                next_state = stringToState(next_buffer);
            } else {
                std::set<char> unique_chars(next_buffer.begin(), next_buffer.end());
                if (unique_chars.size() == ALPHABET_SIZE) {
                    next_state = stringToState(next_buffer.substr(1));
                } else {
                    next_state = FAIL_STATE;
                }
            }
            dfa[state][char_id] = next_state;
        }
    }

    for (int char_id = 0; char_id < ALPHABET_SIZE; char_id++) {
        dfa[FAIL_STATE][char_id] = FAIL_STATE;
    }

    return dfa;
}

// Problem 1: Counts the number of void strings of length n
Big problem1(int n, const std::vector<std::vector<int>>& dfa) {
    std::vector<Big> prev_count(NUM_TOTAL_STATES, 0);
    std::vector<Big> next_count(NUM_TOTAL_STATES, 0);

    prev_count[0] = 1;

    for (int i = 0; i < n; i ++) {
        std::fill(next_count.begin(), next_count.end(), Big(0));
        for (int state = 0; state < FAIL_STATE; state++) {
            if (prev_count[state] > 0) {
                for (int char_id = 0; char_id < ALPHABET_SIZE; char_id++) {
                    int next_state = dfa[state][char_id];
                    if (next_state != FAIL_STATE) {
                        next_count[next_state] += prev_count[state];
                    }
                }
            }
        }
        prev_count.swap(next_count);
    }

    Big result = 0;
    for (int i = 0; i < FAIL_STATE; i++) {
        result += prev_count[i];
    }
    return result;
}

// Problem 2: Counts the number of strings of even length n that are in L and have
// "aa" as the middle two symbols
Big problem2(int n, const std::vector<std::vector<int>>& dfa) {
    if (n % 2 == 1 || n < 2) {
        return 0;
    }
    const int half = n / 2;
    const int steps = half - 1;

    std::vector<Big> left_prev(NUM_TOTAL_STATES, 0), left_next(NUM_TOTAL_STATES, 0);
    left_prev[0] = 1;

    for (int t = 0; t < steps; ++t) {
        std::fill(left_next.begin(), left_next.end(), Big(0));
        for (int s = 0; s < FAIL_STATE; ++s) {
            if (left_prev[s] == 0) {
                continue;
            }
            for (int a = 0; a < ALPHABET_SIZE; ++a) {
                int ns = dfa[s][a];
                if (ns != FAIL_STATE) left_next[ns] += left_prev[s];
            }
        }
        left_prev.swap(left_next);
    }

    std::vector<Big> acc_prev(NUM_TOTAL_STATES, 0), acc_next(NUM_TOTAL_STATES, 0);

    for (int s = 0; s < FAIL_STATE; ++s) {
        acc_prev[s] = 1;
    }

    for (int t = 0; t < steps; ++t) {
        std::fill(acc_next.begin(), acc_next.end(), Big(0));
        for (int s = 0; s < NUM_TOTAL_STATES; ++s) {
            Big sum = 0;
            for (int a = 0; a < ALPHABET_SIZE; ++a) {
                sum += acc_prev[dfa[s][a]];
            }
            acc_next[s] = sum;
        }
        acc_prev.swap(acc_next);
    }

    Big ans = 0;

    for (int p = 0; p < FAIL_STATE; ++p) {
        Big leftways = left_prev[p];
        if (leftways == 0) {
            continue;
        }
        int s1 = dfa[p][0]; // δ(p,'a')
        int s2 = (s1 == FAIL_STATE) ? FAIL_STATE : dfa[s1][0]; // δ(p,"aa")
        if (s2 != FAIL_STATE) {
            ans += leftways * acc_prev[s2];
        }
    }
    return ans;
}

int main() {
    auto dfa = dfa_builder();

    int choice = 0;

    while (true) {
        std::cout << "1. Problem 1\n";
        std::cout << "2. Problem 2\n";
        std::cout << "4. TERMINATE\n";
        std::cin >> choice;

        if (choice == 1) {
            int n;
            std::cout << "Enter string length (n): ";
            std::cin >> n;

            if (n >= 0) {
                Big result = problem1(n, dfa);
                std::cout << "Valid strings of length " << n << ": " << result << std::endl;
            } else {
                std::cout << "Invalid string length" << std::endl;
            }
        } else if (choice == 2) {
            int n;
            std::cout << "Enter string length (n, must be even): ";
            std::cin >> n;
            if (n >= 0) {
                Big result = problem2(n, dfa);
                std::cout << "Valid strings of length " << n
                          << " with middle 'aa': " << result << std::endl;
            } else {
                std::cout << "Invalid string length" << std::endl;
            }
        } else if (choice == 4) {
            std::cout << "Terminating..." << std::endl;
            break;
        }
    }
    return 0;
}