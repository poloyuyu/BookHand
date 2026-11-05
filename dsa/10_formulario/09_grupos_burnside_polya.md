## 9. Teoría de Grupos, Burnside y Pólya

### 9.1 Definiciones básicas

- **Grupo** $(G,·)$: asociativa, neutro $e$, inversos. $|G|$: **cardinalidad del grupo**.
- **Orden de un elemento:** $ord(g) = min{n>0 : g^n = e}$; siempre $ord(g) ∣ |G|$.
- **Subgrupo** $H ≤ G$: cerrado bajo $·$ e inversos.
- **Cíclico:** $G = ⟨g⟩$. $ℤ_n$ es el modelo.
- **Generadores:** $g$ genera $G ⟺ ord(g) = |G|$. #generadores de $ℤ_n = φ(n)$.

### 9.2 Teoremas

> **Lagrange.** $H ≤ G$ finito $⟹ |H| ∣ |G|$. En particular $ord(g) ∣ |G|$.

> **Orden primo.** $|G| = p$ primo $⟹ G ≅ ℤ_p$.

> **Unidades.** $(ℤ/nℤ)^*$ cíclico $⟺ n ∈ {1,2,4,p^k,2p^k}$. Si sí, tiene $φ(φ(n))$ generadores.

### 9.3 Acción de grupo, órbita-estabilizador

$G$ finito actúa sobre $X$. $Orb(x) = {g · x : g ∈ G}$, $Stab(x) = {g : g · x = x}$.

> $$|Orb(x)| · |Stab(x)| = |G|.$$

### 9.4 Lema de Burnside

> **Burnside.** Sea $G$ finito actuando sobre $X$. El número de órbitas es
> $$|X/G| = (1/|G|) ∑_(g ∈ G) |X^g|,$$
> donde $|G|$ es la **cardinalidad del grupo** (número de simetrías consideradas), $X^g = {x ∈ X : g · x = x}$ el conjunto de puntos fijos de $g$, y $X/G$ el conjunto de configuraciones distintas salvo simetría.

**Ejemplo ($ℤ_n$ rotaciones):** collares de $n$ cuentas con $k$ colores,

$$# = (1/n) ∑_(d ∣ n) φ(d) k^(n/d).$$

### 9.5 Pólya (Burnside con pesos)

Sea $j_i(g) =$ #ciclos de longitud $i$ de la permutación de $X$ inducida por $g$. Índice de ciclos:

$$Z(G; x_1,…,x_n) = (1/|G|) ∑_(g ∈ G) ∏_(i=1..n) x_i^(j_i(g)).$$

Coloraciones con $k$ colores: $Z(G; k,…,k)$.

- $ℤ_n$: $Z = (1/n) ∑_(d ∣ n) φ(d) x_d^(n/d)$.
- $D_n$ ($2n$ elementos):
$$Z(D_n) = (1/2)Z(ℤ_n) + { (1/2)x_1 x_2^((n-1)/2) si n impar;  (1/4)(x_1^2 x_2^((n-2)/2) + x_2^(n/2)) si n par }.$$
