# Sudoku Solver with GUI
Multi-threaded Sudoku solver with GUI I hacked together quickly to learn about [imgui](https://github.com/ocornut/imgui).
It uses the recursive backtracking algorithm used by many other Sudoku solver implementations, while rendering the board in parallel with the aid of a worker thread.
![Screenshot](/screenshot.png)

Note that it looks for `C:\Windows\Fonts\corbel.ttf` in runtime during initialization, and won't render the correct font if the file is not found.
