## 10. Geometría Computacional

### 10.1 Vectores

$u = (u_x, u_y)$; $|u| = √(u_x^2+u_y^2)$. Punto: $u · v = u_x v_x + u_y v_y = |u||v|cos θ$. Cruz 2D: $u × v = u_x v_y - u_y v_x = |u||v|sin θ$. Orientación de $A,B,C$: $sgn((B-A)×(C-A))$.

### 10.2 Áreas

- **Shoelace:** $A = (1/2)|∑_i (x_i y_(i+1) - x_(i+1) y_i)|$.
- **Pick:** $A = I + B/2 - 1$ (vértices en $ℤ^2$, polígono simple).
- Triángulo: $A = (1/2)|(B-A)×(C-A)|$.

### 10.3 Intersecciones y distancias

- **Segmentos:** $AB ∩ CD ≠ ∅$ si $sgn(AB × AC) · sgn(AB × AD) ≤ 0$ y análogo con $CD$ (manejar colinealidad aparte).
- Punto-recta ($A,B$): $d = |(B-A)×(P-A)| / |B-A|$.
- Punto-segmento: proyectar $t = ((P-A)·(B-A)) / |B-A|^2$, clamp $t ∈ [0,1]$.

### 10.4 Círculos

$(x-c_x)^2 + (y-c_y)^2 = r^2$. Intersección de dos círculos si $|r_1-r_2| ≤ d ≤ r_1+r_2$ ($d$ distancia entre centros); área vía dos segmentos circulares (fórmula de la *lens*).

### 10.5 Convex hull

- **Monotone chain (Andrew):** ordenar por $(x,y)$, construir cadena inferior y superior; $O(n log n)$.
- **Graham scan:** ordenar por ángulo, pila; $O(n log n)$.
