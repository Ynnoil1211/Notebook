# build_pdf.ps1 — Script de compilación rápida para PowerShell
param(
    [switch]$NoPandoc,
    [switch]$Watch
)

$argsList = @()
if ($NoPandoc) { $argsList += "--no-pandoc" }
if ($Watch)    { $argsList += "--watch" }

python build_pdf.py @argsList
