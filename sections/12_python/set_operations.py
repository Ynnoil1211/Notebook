A = {1, 2, 3, 4}
B = {3, 4, 5, 6}

inter = A & B       # Interseccion: {3, 4}
union = A | B       # Union: {1, 2, 3, 4, 5, 6}
dif   = A - B       # En A pero no en B: {1, 2}
simet = A ^ B       # Diferencia simetrica: {1, 2, 5, 6}
sub   = A <= B      # True si A es subconjunto de B
