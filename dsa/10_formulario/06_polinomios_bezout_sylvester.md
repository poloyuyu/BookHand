## 6. Polinomios, Bézout y Sylvester

### 6.1 Operaciones básicas

Sobre $R[x]$, $R$ cuerpo (o dominio):

- División: $A = BQ + R$, $deg R < deg B$. GCD polinomial por Euclides.
- **Euclides extendido polinomial:** $A · s + B · t = gcd(A,B)$.

### 6.2 Teorema de Bézout

> **Bézout para polinomios.** $gcd(A,B)=1$ en $K[x] ⟺ ∃ s,t ∈ K[x]$ con $As+Bt=1$.

Raíz $α$ de $A ⟺ (x-α) ∣ A$. Múltiples raíces $⟺ gcd(A,A')$ no constante.

### 6.3 Resultante y discriminante

La **matriz de Sylvester** $S(A,B)$ de $A = ∑_(i=0..m) a_i x^i$, $B = ∑_(j=0..n) b_j x^j$ es la $(m+n) × (m+n)$ con filas de coeficientes de $A$ y $B$ desplazados. Se define

$$Res(A,B) = det S(A,B).$$

Propiedades: $Res(A,B) = (-1)^(mn) Res(B,A) = a_m^n ∏_(B(β_i)=0) A(β_i)$. Además $A,B$ tienen raíz común $⟺ Res(A,B) = 0$.

**Discriminante:** $disc(A) = (-1)^(C(m,2)) a_m^(-1) Res(A,A')$.

### 6.4 Interpolación (Lagrange)

Dados $n+1$ puntos $(x_i,y_i)$ con $x_i$ distintos, el polinomio de grado $≤ n$ que los interpola es

$$P(x) = ∑_(i=0..n) y_i ∏_(j ≠ i) (x-x_j)/(x_i-x_j).$$

En puntos $0,1,…,n$, usar prefix/suffix para $O(n)$ por evaluación. Newton: usar diferencias divididas, útil si se agregan puntos.
