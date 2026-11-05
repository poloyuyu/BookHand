## 4. Recurrencias, Matrices y Transformaciones Lineales

### 4.1 Recurrencias lineales homogéneas

$a_n = c_1 a_(n-1) + ⋯ + c_d a_(n-d)$. Polinomio característico $P(λ) = λ^d - c_1 λ^(d-1) - ⋯ - c_d$.

- Si las raíces $λ_i$ son simples: $a_n = ∑ α_i λ_i^n$, coeficientes de condiciones iniciales.
- Raíz $λ$ de multiplicidad $m$: contribución $(β_0+β_1 n+⋯+β_(m-1)n^(m-1))λ^n$.
- Inhomogénea $a_n = ∑ c_i a_(n-i) + f(n)$: suma la solución particular (polinomio si $f$ polinomial; $λ^n · poly(n)$ si $f$ exponencial).

### 4.2 Exponenciación de matrices

Para $a_n$ con recurrencia de orden $d$, escribir $v_n = (a_n, a_(n-1), …, a_(n-d+1))^T = A v_(n-1)$, con

$$A = [c_1, c_2, …, c_d;  1, 0, …, 0;  ⋮, ⋱, ⋮;  0, …, 1, 0].$$

Entonces $v_n = A^(n-d+1) v_(d-1)$, calculado por exponenciación binaria en $O(d^3 log n)$ (o $O(d^2 log n)$ con Berlekamp–Massey + Kitamasa).

### 4.3 Berlekamp–Massey

Dada una secuencia $s_0, s_1, …$ generada por una recurrencia lineal de orden $≤ L$, encuentra la recurrencia mínima en $O(n^2)$. Permite calcular $s_N$ con Kitamasa en $O(L^2 log N)$ o $O(L log L log N)$ con FFT. Condición: el orden es menor a $∼ n/2$.

### 4.4 Transformaciones lineales

- $T: ℝ^n → ℝ^m$ lineal $⟺ T(α x + β y) = α T(x) + β T(y)$.
- Se representa por matriz $A ∈ ℝ^(m×n)$ con $T(x) = Ax$.
- **Composición:** $T_2 ∘ T_1 ⟷ A_2 A_1$. **Inversa:** $T^(-1) ⟷ A^(-1)$ (si $det A ≠ 0$).
- **Cambio de base** $P$: $A' = P^(-1) A P$. $det A' = det A$, $tr A' = tr A$, espectro invariante.

### 4.5 Determinantes

- $det A = ∑_(σ ∈ S_n) sgn(σ) ∏_i A_(i,σ(i))$.
- Propiedades: multilineal por filas, $det A = det A^T$, $det(AB) = det A det B$, $det(A^(-1)) = 1/det A$.
- **Cramer:** $x_i = det A_i / det A$, $A_i =$ $A$ con columna $i$ reemplazada por $b$.
- **Cofactor:** $C_(ij) = (-1)^(i+j) M_(ij)$ ($M_(ij) =$ determinante al tachar fila $i$, col $j$); $det A = ∑_j A_(ij) C_(ij)$. $A^(-1)_(ij) = C_(ji)/det A$.
- **Vandermonde:** $det(x_i^(j-1))_(i,j) = ∏_(i<j)(x_j-x_i)$.
- **Cauchy:** $det(1/(x_i+y_j)) = ∏_(i<j)(x_j-x_i)(y_j-y_i) / ∏_(i,j)(x_i+y_j)$.
- **Circulante:** si $c = (c_0,…,c_(n-1))$, $det C = ∏_(k=0..n-1)(∑_j c_j ω^(jk))$ con $ω = e^(2πi/n)$.
- **Lema de matriz:** $det(I+uv^T) = 1+v^Tu$.
- **Sylvester:** $det(I_m+AB) = det(I_n+BA)$ (compatibilidad adecuada).
