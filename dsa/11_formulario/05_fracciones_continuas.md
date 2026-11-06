## 5. Fracciones Continuas

### 5.1 Expansión

$$x = a_0 + 1/(a_1 + 1/(a_2 + ⋯)),   a_i ∈ ℤ,   a_i > 0 para i ≥ 1.$$

Se denota $[a_0; a_1, a_2, …]$. Algoritmo: $a_0 = ⌊x⌋$, $x ← 1/(x-a_0)$.

### 5.2 Convergentes

$p_(-2)=0$, $p_(-1)=1$, $q_(-2)=1$, $q_(-1)=0$; recurrencias

$$p_i = a_i p_(i-1) + p_(i-2),   q_i = a_i q_(i-1) + q_(i-2).$$

El convergente $p_i/q_i$ satisface $p_i q_(i-1) - p_(i-1) q_i = (-1)^(i-1)$. Es una fracción **reducida** y la mejor aproximación racional con denominador $≤ q_i$.

### 5.3 Aplicaciones

- **Ecuación de Pell** $x^2 - Ny^2 = 1$: soluciones = convergentes de $√N$ con periodo par.
- **Aproximación racional** de $x$: truncar en $q_i ≤ B$.
- **Stern–Brocot:** árbol que contiene todo $ℚ_(>0)$; descenso por comparación con mediana.
