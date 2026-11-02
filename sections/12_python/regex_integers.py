import re

s = "pos: (x=15, y=-42), vel=[+100, -5], count=8"
# Extrae TODOS los enteros con o sin signo en 1 linea:
nums = [int(x) for x in re.findall(r'-?\d+', s)]
# nums = [15, -42, 100, -5, 8]
