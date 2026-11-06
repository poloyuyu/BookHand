## 7. Series Formales, Abel y Sumas Hipergeométricas

### 7.1 Series de potencias

- Geométrica: $∑_(n ≥ 0) x^n = 1/(1-x)$. Derivada: $∑_(n ≥ 1) n x^n = x/(1-x)^2$.
- Formal: definir $F(x) = ∑ a_n x^n$; suma y producto por convolución ordinaria.
- $[x^n]F(x) = F^((n))(0)/n! = (1/(2πi)) ∮ F(z)/z^(n+1) dz$ (Cauchy).

### 7.2 Teorema de Abel

Si $∑ a_n$ converge con suma $S$, entonces $lim_(x → 1^-) ∑ a_n x^n = S$. (En sentido inverso, requiere condiciones de Tauber.) **Fórmula de sumación de Abel:**

$$∑_(k=1..n) a_k b_k = A_n b_n + ∑_(k=1..n-1) A_k(b_k - b_(k+1)),   A_k = ∑_(i=1..k) a_i.$$

Análoga a integración por partes, útil para convergencia condicional.

### 7.3 Convoluciones

- **Ordinaria:** $(f*g)(n) = ∑_(i=0..n) f(i)g(n-i)$; OGF multiplica.
- **Dirichlet:** $(f*g)(n) = ∑_(d ∣ n) f(d)g(n/d)$; multiplicativa si $f,g$ lo son.
- **Möbius:** $g = f * 1 ⟺ f = g * μ$.
- **$gcd$-convolución:** $(f ⋆_gcd g)(n) = ∑_(gcd(i,j)=n) f(i)g(j)$; calcular con zeta de múltiplos $F(n) = ∑_(n ∣ d) f(d)$ y luego Möbius de múltiplos.
- **$lcm$-convolución:** análogo con zeta de divisores $F(n) = ∑_(d ∣ n) f(d)$.

### 7.4 Newton generalizado y EGF

$(1+x)^α = ∑_(n ≥ 0) C(α,n) x^n$. Para secuencias, la **función generatriz exponencial** (EGF) $∑ a_n x^n/n!$ convierte convolución binomial en producto; útil para Stirling, Bell, particiones etiquetadas.

### 7.5 Sumas hipergeométricas

Un término $t_k$ es **hipergeométrico** si $t_(k+1)/t_k$ es función racional de $k$ (ej. $C(n,k)$).

- **Pascal:** $C(n,k) = C(n-1, k-1) + C(n-1, k)$.
- **Gosper:** encuentra $z_k$ tal que $t_k = z_(k+1)-z_k$, sumando $∑_(k=a..b) t_k = z_(b+1)-z_a$. Si no existe, la suma no es hipergeométrica.
- **Zeilberger (creativa telescópica):** para $∑_k F(n,k)$ hipergeométrica en $n$, encuentra recurrencia lineal en $n$. Base de `hyper` en Mathematica.
- **Chu–Vandermonde:** $∑_k C(r,k) C(s, n-k) = C(r+s, n)$. Variante: $∑_k C(r,k) C(s, n+k) = C(r+s, r-n)$.
- **Suma de Gauss:** $∑_k C(a,k) C(b,k) = C(a+b, a)$.

### 7.6 Algoritmos sobre series

- **FFT/NTT** para producto de polinomios en $O(n log n)$.
- **Potencias de series (Brent–Kung):** $F(x)^k mod x^N$ en $O(N log N)$ con NTT + log/exp formales.
- Exp/log/raíz formal: derivar, integrar, resolver por Newton.
- Inversión de series: recursión de Newton, $O(N log N)$.
