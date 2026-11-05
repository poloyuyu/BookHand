## 12. Álgebra Lineal y Otros

### 12.1 Eliminación gaussiana

Resolver $Ax=b$ en $O(n^3)$ con pivoteo parcial. $det A =$ producto de pivotes (signo según permutaciones de filas). $A^(-1)$ por Gauss–Jordan.

### 12.2 Cayley y Matrix-Tree

> **Cayley.** #árboles etiquetados sobre $n$ vértices: $n^(n-2)$.

> **Matrix-Tree.** Para grafo no dirigido con laplaciano $L = D-A$, #árboles generadores = det de cualquier menor principal $(n-1)×(n-1)$ de $L$.

### 12.3 Regla de Simpson

$$∫_a^b f(x)dx ≈ ((b-a)/6)[f(a) + 4f((a+b)/2) + f(b)].$$

Compuesta con $n$ par, $h = (b-a)/n$:

$$∫_a^b f ≈ (h/3)[f_0 + 4f_1 + 2f_2 + 4f_3 + ⋯ + 4f_(n-1) + f_n].$$

### 12.4 Números armónicos

$H_n = ∑_(k=1..n) 1/k$; $H_n = ln n + γ + O(1/n)$, $γ ≈ 0.5772$. Usos: $∑_(k=1..n) C(n,k)/k$ tiene forma cerrada con $H_n$; análisis de algoritmos (quicksort: $2H_n-2$). **Identidad:** $∑_(k=1..n) 1/(k(k+1)) = 1 - 1/(n+1)$.

### 12.5 Series de Fourier (para coeficientes)

- Trigonométrica: $f(x) = a_0/2 + ∑_(n ≥ 1)(a_n cos nx + b_n sin nx)$, $a_n = (1/π) ∫_(-π)^π f(x)cos(nx)dx$, análogo $b_n$.
- Compleja: $f(x) = ∑_(n ∈ ℤ) c_n e^(inx)$, $c_n = (1/(2π)) ∫_(-π)^π f(x)e^(-inx)dx$.
- Ortogonalidad: $(1/(2π)) ∫_(-π)^π e^(i(n-m)x)dx = δ_(nm)$.
