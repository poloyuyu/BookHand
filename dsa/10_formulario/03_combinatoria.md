## 3. Combinatoria

### 3.1 Coeficientes y binomios

- $C(n,k) = n! / (k!(n-k)!)$. $C(n; n_1,…,n_r) = n! / (n_1!⋯n_r!)$.
- **Newton generalizado:** $(1+x)^α = ∑_(n ≥ 0) C(α,n) x^n$, con $C(α,n) = α(α-1)⋯(α-n+1)/n!$ (válido para $α ∈ ℝ$ formalmente).
- **Vandermonde:** $C(m+n, k) = ∑_i C(m,i) C(n, k-i)$.
- **Hockey-stick:** $∑_(i=r..n) C(i,r) = C(n+1, r+1)$.
- Recurrencia: $C(n,k) = C(n-1, k-1) + C(n-1, k)$.
- Identidades: $∑_k C(n,k) = 2^n$; $∑_k C(n,k)^2 = C(2n, n)$; $∑_k k C(n,k) = n 2^(n-1)$; $∑_k (-1)^k C(n,k) = [n=0]$.
- $C(n,k) ≡ C(n mod p, k mod p) C(⌊n/p⌋, ⌊k/p⌋) (mod p)$ (Lucas).

### 3.2 Stars and bars

Soluciones de $x_1+⋯+x_k=n$:

- $x_i ≥ 0$: $C(n+k-1, k-1)$.
- $x_i ≥ 1$: $C(n-1, k-1)$.
- $x_i ≥ a_i$: sustituir $x_i' = x_i - a_i$.

### 3.3 Números especiales

- **Catalan:** $C_n = C(2n,n)/(n+1)$, $C_(n+1) = ∑ C_i C_(n-i)$, $C_0 = 1$.
- **Stirling 2ª:** $S(n,k) = (1/k!) ∑_(j=0..k) (-1)^(k-j) C(k,j) j^n$; $S(n,k) = S(n-1,k-1) + k S(n-1,k)$. Inversión: $x^n = ∑ S(n,k) x^(k↓)$.
- **Stirling 1ª (sin signo)** $c(n,k)$: $x^(n↑) = ∑ c(n,k)x^k$; $c(n,k) = c(n-1,k-1)+(n-1)c(n-1,k)$. Con signo $s(n,k) = (-1)^(n-k)c(n,k)$, $x^(n↓) = ∑ s(n,k)x^k$.
- **Bell:** $B_n = ∑_k S(n,k)$; $B_(n+1) = ∑_k C(n,k) B_k$; EGF $exp(e^x-1)$.
- **Eulerianos** $A(n,k)$: permutaciones con $k$ ascensos; $A(n,k) = (n-k)A(n-1,k-1)+(k+1)A(n-1,k)$, $∑_k A(n,k) = n!$. Explícito: $A(n,k) = ∑_(j=0..k+1) (-1)^j C(n+1,j)(k+1-j)^n$.
- **Derangements:** $D_n = n! ∑_(k=0..n) (-1)^k / k!$, $D_n = (n-1)(D_(n-1)+D_(n-2))$, $D_n = n D_(n-1)+(-1)^n$.
- **Particiones** $p(n)$: OGF $∏_(k ≥ 1) 1/(1-x^k) = ∑ p(n)x^n$. Euler pentagonal: $∏_(k ≥ 1)(1-x^k) = ∑_(j ∈ ℤ) (-1)^j x^(j(3j-1)/2)$. Asintótico $p(n) ∼ e^(π√(2n/3)) / (4n√3)$.
- **Números de Lah** $L(n,k) = C(n-1, k-1) n! / k!$ (particiones ordenadas).
- **Números de Narayana** $N(n,k) = C(n,k) C(n,k-1) / n$ (permutaciones con $k$ runs).
- **Números de Schröder** (caminos con pasos $(1,0),(0,1),(1,1)$): OGF $S(x) = (1-x-√(x^2-6x+1))/(2x)$.
- **Números de Motzkin** $M_n$: pasos ${(1,0),(1,1),(1,-1)}$ sin bajar; $∑ M_n x^n = (1-x-√(1-2x-3x^2))/(2x^2)$.
- **Fubini (ordenados Bell)** $F_n = ∑_k k! S(n,k)$; EGF $1/(2-e^x)$.
- **Fibonacci:** $F_n = (φ^n-ψ^n)/√5$, $φ = (1+√5)/2$, $ψ = (1-√5)/2$. $∑_(i=0..n) F_i = F_(n+2)-1$; $∑ F_i^2 = F_n F_(n+1)$.
- **Lucas:** $L_n = F_(n-1)+F_(n+1) = φ^n+ψ^n$.

### 3.4 Inclusión-exclusión

$$|⋃_(i=1..n) A_i| = ∑|A_i| - ∑|A_i ∩ A_j| + ⋯ + (-1)^(n-1)|A_1 ∩ ⋯ ∩ A_n|.$$

**Forma general:** si $A_i$ es el evento de "tener propiedad $i$", entonces

$$#{sin ninguna propiedad} = ∑_(S ⊆ [n]) (-1)^(|S|) |⋂_(i ∈ S) A_i|.$$

### 3.5 Sumas de potencias

- $∑_(k=1..n) k = n(n+1)/2$.
- $∑_(k=1..n) k^2 = n(n+1)(2n+1)/6$.
- $∑_(k=1..n) k^3 = (n(n+1)/2)^2$.
- **Faulhaber:** $∑_(k=1..n) k^p = (1/(p+1)) ∑_(j=0..p) (-1)^j C(p+1,j) B_j n^(p+1-j)$ con $B_j$ números de Bernoulli.
- $B_0=1$, $B_1=-1/2$, $B_2=1/6$, $B_4=-1/30$, $B_6=1/42$. OGF: $x/(e^x-1) = ∑ B_n x^n/n!$.
