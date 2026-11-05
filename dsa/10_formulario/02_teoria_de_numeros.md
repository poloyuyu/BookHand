## 2. Teoría de Números

### 2.1 Algoritmos básicos

- **Euclides:** $gcd(a,b) = gcd(b, a mod b)$, $gcd(a,0)=a$. Costo $O(log min(a,b))$.
- **Euclides extendido:** devuelve $(g,x,y)$ con $g=gcd(a,b)$ y $ax+by=g$. Si $gcd(a,m)=1$, $x mod m = a^(-1)$.
- **Exponenciación rápida:** $a^n mod m$ en $O(log n)$ por cuadrados sucesivos.
- **Criba de Eratóstenes:** $O(N log log N)$. **Criba linear:** $O(N)$. **Segmentada:** $O((R-L)log log R + √R)$.

### 2.2 Inverso modular y congruencias lineales

- $a^(-1) (mod m)$ existe $⟺ gcd(a,m)=1$.
- $ax ≡ b (mod n)$ tiene solución $⟺ d=gcd(a,n) ∣ b$. Si sí, hay $d$ soluciones módulo $n$: $x = x_0 + k(n/d)$, $k=0,…,d-1$.
- Inverso sin extgcd: $a^(-1) ≡ a^(φ(m)-1) (mod m)$ (si $m$ primo: $a^(m-2)$).

### 2.3 Invariantes del Euclides extendido

Al correr extgcd$(a,b)$ mantenemos $(r_i,s_i,t_i)$ con $r_i = s_i a + t_i b$ y las recurrencias

$$r_i = r_(i-2) - q_i r_(i-1),   s_i = s_(i-2) - q_i s_(i-1),   t_i = t_(i-2) - q_i t_(i-1).$$

Inicialización: $(a,1,0),(b,0,1)$. Al final $r_k = gcd(a,b)$, y los coeficientes de Bézout son $s_k,t_k$. Invariante: $s_i t_(i-1) - s_(i-1) t_i = (-1)^(i+1)$.

### 2.4 Teorema Chino del Resto (CRT)

> **Teorema (CRT).** Si $m_1,…,m_k$ son coprimos dos a dos y $M=∏ m_i$, el sistema $x ≡ a_i (mod m_i)$ tiene solución única módulo $M$:
> $$x = ∑_(i=1..k) a_i M_i y_i mod M,   M_i = M/m_i,   y_i ≡ M_i^(-1) (mod m_i).$$

**CRT no coprimo:** consistente $⟺ a_i ≡ a_j (mod gcd(m_i,m_j))$ para todo $i,j$. Se resuelve combinando pares (módulo resultante $= lcm$).

### 2.5 Teoremas clásicos

- **Fermat:** $p$ primo, $p ∤ a ⟹ a^(p-1) ≡ 1 (mod p)$.
- **Euler:** $gcd(a,n)=1 ⟹ a^(φ(n)) ≡ 1 (mod n)$.
- **Wilson:** $(p-1)! ≡ -1 (mod p) ⟺ p$ primo. Generalizado: $(n-1)! ≡ -1 (mod n) ⟺ n=1,4$ o $n$ primo.

### 2.6 Funciones aritméticas

- $φ(n) = n ∏_(p ∣ n)(1-1/p)$; $∑_(d ∣ n) φ(d) = n$.
- $μ(n)=0$ si $p^2 ∣ n$; si no, $μ(n)=(-1)^(ω(n))$ ($ω(n)=$ #primos distintos). $∑_(d ∣ n) μ(d) = [n=1]$.
- **Inversión de Möbius:** $g(n) = ∑_(d ∣ n) f(d) ⟺ f(n) = ∑_(d ∣ n) μ(d) g(n/d)$, i.e. $f = g * μ$.
- $n = ∏ p_i^(e_i) ⟹ d(n) = ∏(e_i+1)$, $σ(n) = ∏ (p_i^(e_i+1)-1)/(p_i-1)$.

### 2.7 Residuos cuadráticos, Legendre, Jacobi

- **Euler:** $a$ es QR mód $p$ primo impar $⟺ a^((p-1)/2) ≡ 1$; QNR $⟺ ≡ -1$.
- #QR mód $p$: $(p-1)/2$. Multiplicatividad: $(ab/p) = (a/p)(b/p)$.
- **Reciprocidad cuadrática:** $(p/q)(q/p) = (-1)^(((p-1)/2)((q-1)/2))$.
- Complementarios: $(-1/p) = (-1)^((p-1)/2)$, $(2/p) = (-1)^((p^2-1)/8)$.
- **Jacobi:** $(a/n) = ∏ (a/p_i)^(e_i)$, $n = ∏ p_i^(e_i)$ impar. $(a/n) = 1$ **no** implica QR mód $n$ si $n$ compuesto.
- Reciprocidad Jacobi: $(m/n)(n/m) = (-1)^(((m-1)/2)((n-1)/2))$ para $m,n$ impares coprimos.

### 2.8 Tonelli–Shanks y raíces modulares

Para $a$ QR mód $p$ primo impar:

- $p ≡ 3 (mod 4)$: $x ≡ a^((p+1)/4)$.
- $p ≡ 5 (mod 8)$: $x ≡ a^((p+3)/8) · (a^((p-1)/4) ≡ 1 ? 1 : 2^((p-1)/4))$.
- $p ≡ 1 (mod 8)$: algoritmo general Tonelli–Shanks, $O(log^2 p)$.

**Discrete root** $x^k ≡ a (mod p)$: si $gcd(k,p-1)=1$, $x = a^(k^(-1) mod (p-1))$; si no, reducir a log discreto.

### 2.9 Raíz primitiva y estructura de $(ℤ/nℤ)^*$

- $g$ primitiva mód $n ⟺ ord_n(g) = φ(n)$.
- Existe $⟺ n ∈ {1,2,4,p^k,2p^k}$ ($p$ primo impar). En ese caso hay $φ(φ(n))$ primitivas.
- Estructura general: si $n = 2^e ∏ p_i^(e_i)$,
$$(ℤ/nℤ)^* ≅ (ℤ/2ℤ)^a × (ℤ/2^(e-2)ℤ) × ∏ ℤ/(p_i^(e_i-1)(p_i-1))ℤ$$
con $a = 0,1,2$ para $e<2, e=2, e ≥ 3$.

### 2.10 Logaritmo discreto (BSGS)

Resolver $g^x ≡ h (mod n)$, $gcd(g,n)=1$. Con $m=⌈√n⌉$, escribir $x = im+j$, $0 ≤ j < m$; precalcular $g^j$, buscar $h · (g^(-m))^i$. Costo $O(√n)$.

### 2.11 Primalidad y factorización

- **Miller–Rabin:** $n-1 = d 2^s$ ($d$ impar); test: $a^d ≡ 1$ o $a^(d 2^r) ≡ -1$ para algún $r<s$. Bases seguras $64$ bits: ${2,3,5,7,11,13,17,19,23,29,31,37}$.
- **Pollard Rho:** $f(x)=x^2+c$, Floyd para ciclo; factor $gcd(|x-y|,n)$. $O(n^(1/4))$ esperado.

### 2.12 Teoremas avanzados

> **Lifting the Exponent (LTE).** $p$ primo impar, $p ∣ x-y$, $p ∤ xy$: $v_p(x^n - y^n) = v_p(x-y) + v_p(n)$. Para $p=2$, $x,y$ impares: $v_2(x^n - y^n) = v_2(x-y)$ si $n$ impar; $v_2(x+y)+v_2(x-y)+v_2(n)-1$ si $n$ par. También $p ∣ x+y$, $n$ impar: $v_p(x^n+y^n) = v_p(x+y)+v_p(n)$.

> **Lucas.** $m = ∑ m_i p^i$, $n = ∑ n_i p^i$: $C(m,n) ≡ ∏ C(m_i, n_i) (mod p)$.

> **Kummer.** $v_p(C(m,n)) =$ #acarreos al sumar $n$ y $m-n$ en base $p$.
