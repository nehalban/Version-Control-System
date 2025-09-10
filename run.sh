# Check if g++ is available
if ! command -v g++ &> /dev/null; then
    echo "Error: Please install g++ to compile this project."
    exit 1
fi

rm -f vcs main.o # Clean previous builds

g++ -Wall -Wextra -O2 -o vcs main.cpp # Compile the project


if [ $? -eq 0 ]; then # $? checks the exit status of the last command
    echo "Compilation successful"

    echo "=== How to run ==="
    echo "1. Interactive mode: ./vcs"
    echo "2. With input file: ./vcs < input.txt"
    echo "3. With input redirection: cat input.txt | ./vcs"
    echo ""
    echo "Starting program (Press Ctrl+D to exit)"
    echo ""
    
    ./vcs
else
    echo "Compilation failed"
    exit 1
fi
