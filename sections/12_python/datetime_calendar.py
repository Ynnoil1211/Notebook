import datetime

# 1. Crear fecha (Ano, Mes, Dia)
d1 = datetime.date(2024, 2, 28)
d2 = datetime.date(2026, 10, 25)

# 2. Diferencia en dias:
dias = (d2 - d1).days       # 970 dias exactos

# 3. Sumar dias a una fecha:
futuro = d1 + datetime.timedelta(days=100)

# 4. Dia de la semana: 0 = Lunes, 6 = Domingo
dia_sem = d1.weekday()

# 5. Formatear fecha:
texto = d1.strftime("%Y-%m-%d, %A")
