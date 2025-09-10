# run_tests.ps1 - PowerShell script for VS Code on Windows

# Compile the program
Write-Host "Compiling Time-Travelling File System..." -ForegroundColor Cyan
$compileResult = g++ -std=c++17 -Wall -Wextra -O2 main.cpp -o filesystem.exe 2>&1

if ($LASTEXITCODE -eq 0) {
    Write-Host "Compilation successful!" -ForegroundColor Green
} else {
    Write-Host "Compilation failed!" -ForegroundColor Red
    Write-Host $compileResult
    exit 1
}

# Function to run a test
function Run-Test {
    param(
        [string]$TestName,
        [string]$TestCommands
    )
    
    Write-Host "`n========================================" -ForegroundColor Yellow
    Write-Host "Running Test: $TestName" -ForegroundColor Yellow
    Write-Host "========================================" -ForegroundColor Yellow
    
    # Save commands to a temporary file
    $TestCommands | Out-File -FilePath "test_input.txt" -Encoding UTF8
    
    # Run the test
    Get-Content "test_input.txt" | ./filesystem.exe | Out-File -FilePath "test_output.txt" -Encoding UTF8
    
    # Display output
    Write-Host "Output:" -ForegroundColor Cyan
    Get-Content "test_output.txt"
    
    # Clean up
    Remove-Item "test_input.txt" -Force -ErrorAction SilentlyContinue
    Remove-Item "test_output.txt" -Force -ErrorAction SilentlyContinue
}

# Test 1: Basic Operations
$test1 = @"
CREATE file1
READ file1
INSERT file1 Hello World
READ file1
UPDATE file1 New Content
READ file1
"@
Run-Test "Basic Operations" $test1

# Test 2: Snapshot and Rollback
$test2 = @"
CREATE file2
INSERT file2 Version 0
SNAPSHOT file2 First snapshot
INSERT file2 Version 1
SNAPSHOT file2 Second snapshot
HISTORY file2
ROLLBACK file2 0
READ file2
"@
Run-Test "Snapshot and Rollback" $test2

# Test 3: Branching
$test3 = @"
CREATE file3
INSERT file3 Root
SNAPSHOT file3 Root snapshot
INSERT file3 Branch A
SNAPSHOT file3 Branch A
ROLLBACK file3 0
INSERT file3 Branch B
SNAPSHOT file3 Branch B
HISTORY file3
"@
Run-Test "Branching" $test3

# Test 4: Analytics
$test4 = @"
CREATE recent1
CREATE recent2
CREATE recent3
UPDATE recent1 Content
UPDATE recent3 Content
UPDATE recent2 Content
RECENT_FILES 2
BIGGEST_TREES 2
"@
Run-Test "Analytics" $test4

# Test 5: Edge Cases
$test5 = @"
CREATE edge
SNAPSHOT edge Empty snapshot
ROLLBACK edge
READ edge
ROLLBACK edge 999
CREATE edge
READ nonexistent
"@
Run-Test "Edge Cases" $test5

Write-Host "`n========================================" -ForegroundColor Green
Write-Host "All tests completed!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green

# Interactive mode option
Write-Host "`nDo you want to run in interactive mode? (Y/N)" -ForegroundColor Cyan
$response = Read-Host
if ($response -eq 'Y' -or $response -eq 'y') {
    Write-Host "Starting interactive mode. Type commands or Ctrl+C to exit:" -ForegroundColor Green
    ./filesystem.exe
}