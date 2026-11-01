# build_pdf.ps1 — Script de compilación rápida para PowerShell (OUR BRAINS ARE DIAZ O)
param(
    [switch]$NoPandoc,
    [switch]$Watch,
    [string]$InputFile = "",
    [string]$Template = "",
    [string]$Body = "",
    [string]$Output = ""
)

$scriptPath = Join-Path $PSScriptRoot "build_pdf.py"
$argsList = @()

if ($NoPandoc)            { $argsList += "--no-pandoc" }
if ($Watch)               { $argsList += "--watch" }
if ($InputFile -ne "")    { $argsList += @("--input", $InputFile) }
if ($Template -ne "")     { $argsList += @("--template", $Template) }
if ($Body -ne "")         { $argsList += @("--body", $Body) }
if ($Output -ne "")       { $argsList += @("--output", $Output) }

python -X utf8 $scriptPath @argsList
exit $LASTEXITCODE
