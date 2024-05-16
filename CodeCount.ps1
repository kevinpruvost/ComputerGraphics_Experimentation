$folders = @("ComputerGraphics_Experimentation",
"lib/Venom",
"lib/ContextAPIs",
"lib/GraphicsAPIs",
"lib/GUIAPIs",
)  # Add your folder paths here

$totalLines = 0

foreach ($folder in $folders) {
    $files = Get-ChildItem -Path $folder -Recurse -Include *.h, *.cpp -File

    foreach ($file in $files) {
        $lineCount = (Get-Content $file.FullName -Raw | Measure-Object -Line).Lines
        Write-Host "File: $($file.FullName) Lines: $lineCount"
        $totalLines += $lineCount
    }
}

Write-Host "Total lines of code: $totalLines"