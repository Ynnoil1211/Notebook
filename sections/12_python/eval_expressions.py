# eval() procesa operadores (+, -, *, //, %, **)
expr = "((15 + 25) * 3) // 4"
resultado = eval(expr)      # 30

# Reemplazar solo '/' simples por '//' sin duplicar '//'
# existentes:
import re
expr_safe = re.sub(r"(?<!/)/(?!/)", "//", expr)
