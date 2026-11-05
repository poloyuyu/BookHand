## 8. Funciones Generatrices

### 8.1 Tipos y definición

Dada una sucesión $(a_n)_(n ≥ 0)$, se define:

- **OGF** (ordinaria): $A(x) = ∑_(n ≥ 0) a_n x^n$.
- **EGF** (exponencial): $Â(x) = ∑_(n ≥ 0) a_n x^n / n!$.
- **Dirichlet:** $D_A(s) = ∑_(n ≥ 1) a_n / n^s$ (útil para convolución de Dirichlet).
- **Ordinaria truncada:** $A_N(x) = ∑_(n=0..N) a_n x^n$ (lo que manipulamos mód $x^(N+1)$).

**Relación OGF/EGF:** la OGF de $a_n/n!$ es la EGF de $a_n$; la EGF de $a_n · n!$ es la OGF de $a_n$. En CP, si hay etiquetas (permutaciones, particiones de conjuntos, árboles etiquetados) se usa EGF; si no, OGF.

### 8.2 Operaciones básicas

Con $A(x) = ∑ a_n x^n$, $B(x) = ∑ b_n x^n$:

- **Suma:** $A+B ⟷ a_n+b_n$.
- **Producto (convolución):** $(A · B)(x) ⟷ c_n = ∑_(i=0..n) a_i b_(n-i)$.
- **Derivada:** $A'(x) = ∑_(n ≥ 1) n a_n x^(n-1) ⟷ n a_n$.
- **Integral:** $∫_0^x A(t)dt = ∑_(n ≥ 0) (a_n/(n+1)) x^(n+1)$.
- **Desplazamiento:** $x^k A(x) ⟷ a_(n-k)$ (con $a_(<0)=0$).
- **Escalamiento:** $A(cx) ⟷ c^n a_n$.
- **Producto por $x/(1-x)$:** $A(x)/(1-x) ⟷ ∑_(i=0..n) a_i$ (prefijos).
- **Producto por $1/(1-x^k)$:** inserta bloques de tamaño $k$.
- **Producto por $(1-x)$:** $A(x)(1-x) ⟷ a_n - a_(n-1)$ (diferencias).

### 8.3 Extracción de coeficientes

- $[x^n]A(x) = A^((n))(0)/n!$ (Taylor).
- **Cauchy (contorno):** $[x^n]A(x) = (1/(2πi)) ∮_γ A(z)/z^(n+1) dz$.
- **Extracción racional:** si $A(x) = P(x)/Q(x)$ con $deg P < deg Q$, descomponer en fracciones parciales
$$P(x)/Q(x) = ∑_j ∑_(r=1..m_j) α_(j,r) / (1-λ_j x)^r,$$
y usar $[x^n](1-λ x)^(-r) = C(n+r-1, r-1) λ^n$. Esto resuelve **cualquier** recurrencia lineal homogénea con coeficientes constantes.

### 8.4 Series notables (tabla de referencia)

- $1/(1-x) = ∑ x^n$
- $1/(1-x)^k = ∑ C(n+k-1, k-1) x^n$
- $(1+x)^α = ∑ C(α,n) x^n$
- $e^x = ∑ x^n/n!$, $ln(1+x) = ∑_(n ≥ 1) ((-1)^(n-1)/n) x^n$
- $sin x = ∑ (-1)^n x^(2n+1)/(2n+1)!$, $cos x = ∑ (-1)^n x^(2n)/(2n)!$
- $x/(e^x-1) = ∑ B_n x^n/n!$ (Bernoulli)
- $∏_(k ≥ 1) 1/(1-x^k) = ∑ p(n)x^n$ (particiones)
- $∏_(k ≥ 1)(1+x^k) = ∑ q(n)x^n$ (particiones en partes distintas)
- $1/(1-x-x^2) = ∑ F_(n+1)x^n$ (Fibonacci)
- $(1-x-√(1-2x-3x^2))/(2x^2) = ∑ M_n x^n$ (Motzkin)
- $C(x) = (1-√(1-4x))/(2x) = ∑ C_n x^n$ (Catalan)

### 8.5 Funciones generatrices clásicas derivadas

- **Catalan:** $C(x) = 1+xC(x)^2 ⟹ C(x) = (1-√(1-4x))/(2x)$.
- **Fibonacci:** $F(x) = x/(1-x-x^2)$. Binet: $F_n = (φ^n-ψ^n)/√5$.
- **Bell (EGF):** $∑ B_n x^n/n! = exp(e^x-1)$.
- **Stirling 2ª (EGF en $n$, OGF en $k$):** $∑_(n ≥ k) S(n,k) x^n/n! = (e^x-1)^k / k!$.
- **Stirling 1ª sin signo (EGF):** $∑_(n ≥ k) c(n,k) x^n/n! = (-ln(1-x))^k / k!$.
- **Derangements (EGF):** $∑ D_n x^n/n! = e^(-x)/(1-x)$.
- **Eulerianos:** $∑_(n ≥ 0) A(n,k) x^n/n! = x^(k+1)/(1-x)^(k+1)$ (como serie en $x$).
- **Particiones:** $P(x) = ∏_(k ≥ 1) 1/(1-x^k)$. Pentagonal: $∏(1-x^k) = ∑_j (-1)^j x^(j(3j-1)/2)$.

### 8.6 Composición, inversa y Lagrange

Si $A(x) = ∑ a_n x^n$ con $a_0=0$, $a_1 ≠ 0$, existe la **inversa composicional** $A^(⟨-1⟩)(x)$ tal que $A(A^(⟨-1⟩)(x)) = x$.

> **Inversión de Lagrange.** Si $A(x) = x ϕ(A(x))$ con $ϕ(0) ≠ 0$, entonces
> $$[x^n]A(x) = (1/n)[t^(n-1)]ϕ(t)^n.$$
> Equivalentemente, si $w = x ϕ(w)$:
> $$[x^n]f(w(x)) = (1/n)[t^(n-1)]f'(t)ϕ(t)^n.$$

**Uso típico:** contar árboles (Cayley: $n^(n-2)$), obtener coeficientes de la inversa de una serie dada, resolver ecuaciones funcionales $A = x e^A$ (árboles etiquetados) o $A = x(1+A)^k$ (Fuss–Catalan).

### 8.7 Composición y potencias de series (algoritmia)

Trabajando mód $x^N$:

- **Inversa multiplicativa** $1/A$ (con $a_0 ≠ 0$): recursión de Newton $B_(2k) = B_k(2-AB_k)$, $O(N log N)$.
- **Logaritmo formal:** $log A = ∫ A'/A$.
- **Exponencial formal:** $exp A$ por Newton: $B_(2k) = B_k(1-log B_k + A)$.
- **Raíz $k$-ésima:** $A^(1/k) = exp((1/k) log A)$.
- **Potencia general:** $A^α = exp(α log A)$ (requiere $a_0=1$).
- **Composición** $A(B(x))$: $O(N^(1.5))$ con bloque de Brent–Kung; rara en CP, pero existe.

Todas usan NTT/FFT para $O(N log N)$ por operación.

### 8.8 Ecuaciones funcionales y coeficientes

Método general: si $A(x) = F(x, A(x))$ con $F$ analítica:

1. Escribir $A = a_0 + a_1 x + ⋯$ e igualar coeficientes de $x^n$.
2. Si $A = xϕ(A)$: aplicar Lagrange.
3. Si $A = F(A/x)$: sustituir $B = A/x$.
4. Si $A = 1+xA^k$: Fuss–Catalan, $a_n = C(kn, n) / ((k-1)n+1)$.

### 8.9 Bivariadas y extracción diagonal

Para sucesiones $a_(n,k)$, usar $A(x,y) = ∑ a_(n,k)x^n y^k$.

- **Diagonal:** $∑ a_(n,n)x^n = [y^0]A(x/y,y)$. Extraíble con residuos.
- **Coeficiente binomial:** $∑_k C(n,k)x^k y^(n-k) = (x+y)^n$.
- **Delannoy:** $D(x,y) = 1/(1-x-y-xy)$.

### 8.10 Convoluciones como GFs

- **Ordinaria:** producto $A(x)B(x)$.
- **Binomial:** $∑_k C(n,k)a_k b_(n-k) ⟷$ producto de EGF.
- **Dirichlet:** producto de series de Dirichlet, $(f*g)(n) = ∑_(d ∣ n) f(d)g(n/d)$.
- **Möbius:** $g = f * 1 ⟺ f = g * μ$; en GFs de Dirichlet: $G = ζ · F$, $F = G/ζ = G · (1/ζ)$, con $1/ζ(s) = ∑ μ(n)n^(-s)$.
- **Euler transform:** $∏_(k ≥ 1)(1-x^k)^(-b_k) = ∑ a_n x^n$, con $a_n = (1/n) ∑_(k=1..n)(∑_(d ∣ k) d b_d)a_(n-k)$. Inversa: $log A = ∑_k b_k ∑_(j ≥ 1) x^(kj)/j$.

### 8.11 Series de Fourier y extracción (puente)

La EGF/OGF a veces se extrae vía **transformada de Fourier discreta** (raíces $n$-ésimas de la unidad):

$$a_n = (1/N) ∑_(j=0..N-1) ω^(-nj) A(ω^j),   ω = e^(2πi/N),   N>n.$$

Base de la **multiplicación con NTT** y del **multipoint evaluation**. **Fourier compleja:** $f(x) = ∑_(n ∈ ℤ) c_n e^(inx)$, $c_n = (1/(2π)) ∫_(-π)^π f(x)e^(-inx)dx$.
