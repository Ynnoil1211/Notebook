// Plantilla maestra de Typst para ICPC Notebook en 3 columnas (Landscape)
// Equipo: OUR BRAINS ARE DIAZ O
// Compilación automática recomendada: python build_pdf.py

#set page(
  paper: "a4",
  flipped: true, // Horizontal (Landscape) ideal para 3 columnas
  margin: (top: 1.05cm, bottom: 0.8cm, left: 0.7cm, right: 0.7cm),
  header-ascent: 22%,
  header: context {
    if counter(page).get().first() > 0 [
      #set text(font: "Segoe UI", size: 7pt, fill: luma(90))
      #block(
        width: 100%,
        inset: (bottom: 2.5pt),
        stroke: (bottom: 0.5pt + luma(160)),
        [*OUR BRAINS ARE DIAZ O — ICPC Team Notebook* #h(1fr) *Página #counter(page).display("1 / 1", both: true)*]
      )
    ]
  }
)

#set text(font: "Segoe UI", size: 7.0pt, lang: "es")
#set par(justify: true, leading: 0.40em, spacing: 0.50em)

// Estilos de encabezados con sticky: true para evitar encabezados huérfanos al final de columna
#show heading.where(level: 1): it => {
  set text(size: 10.5pt, weight: "bold", fill: rgb("#1e3a8a"))
  block(below: 5pt, above: 6pt, sticky: true, it.body)
}

#show heading.where(level: 2): it => {
  set text(size: 8.5pt, weight: "bold", fill: rgb("#ffffff"))
  block(
    fill: rgb("#1e3a8a"),
    inset: (x: 5pt, y: 3.5pt),
    radius: 2pt,
    width: 100%,
    above: 7pt,
    below: 4pt,
    sticky: true,
    it.body
  )
}

#show heading.where(level: 3): it => {
  set text(size: 7.5pt, weight: "bold", fill: rgb("#0f172a"))
  block(
    fill: rgb("#e2e8f0"),
    stroke: (left: 2.5pt + rgb("#2563eb")),
    inset: (x: 4pt, y: 2.5pt),
    width: 100%,
    above: 6pt,
    below: 3.5pt,
    sticky: true,
    it.body
  )
}

#show heading.where(level: 4): it => {
  set text(size: 7.2pt, weight: "bold", fill: rgb("#1d4ed8"))
  block(above: 4pt, below: 2.5pt, sticky: true, it.body)
}

// Bloques de código con Syntax Highlighting nativo de Typst
#show raw.where(block: true): set text(font: "Consolas", size: 5.9pt)
#show raw.where(block: true): set par(justify: false, leading: 0.35em)
#show raw.where(block: true): it => block(
  fill: rgb("#f8fafc"),
  stroke: (left: 1.8pt + rgb("#3b82f6"), rest: 0.35pt + rgb("#cbd5e1")),
  inset: 4pt,
  radius: 2pt,
  width: 100%,
  breakable: it.lang != "text",
  it
)

// Código inline
#show raw.where(block: false): box.with(
  fill: rgb("#f1f5f9"),
  inset: (x: 1.8pt, y: 0pt),
  outset: (y: 1.2pt),
  radius: 1.5pt
)
#show raw.where(block: false): set text(font: "Consolas", size: 6.3pt, fill: rgb("#0f172a"))

// Tablas compactas y rompibles entre columnas para diseño de 3 columnas
#show figure.where(kind: table): set block(breakable: true)
#show figure.where(kind: table): it => block(breakable: true, width: 100%, above: 4pt, below: 4pt, it.body)
#show table: set text(size: 5.9pt, hyphenate: false)
#show table: set par(justify: false, leading: 0.34em)
#set table(
  stroke: 0.4pt + rgb("#cbd5e1"),
  inset: (x: 2.6pt, y: 2.3pt),
  fill: (x, y) => if y == 0 { rgb("#e2e8f0") } else if calc.even(y) { rgb("#f8fafc") } else { none }
)

// Diseño en 3 columnas
#show: columns.with(3, gutter: 0.55cm)

// Índice automático con numeración de página real
#block(
  fill: rgb("#f8fafc"),
  stroke: 0.5pt + rgb("#cbd5e1"),
  inset: 5pt,
  radius: 2pt,
  width: 100%,
  [
    #set text(size: 6.4pt)
    #show outline.entry.where(level: 1): set text(weight: "bold")
    #show regex("Bloque(\s+\d+:?)?"): set text(weight: "bold", fill: rgb("#2563eb"))
    #outline(title: [Índice General con Páginas], depth: 3, indent: 0.8em)
  ]
)

// Contenido generado desde Markdown (admite --input body=... desde build_pdf.py)
#include sys.inputs.at("body", default: "body.typ")
