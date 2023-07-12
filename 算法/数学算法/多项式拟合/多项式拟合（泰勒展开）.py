#该程序展示了用15次多项式去拟合函数$f(x) = \int_0^xe^{-x^2}dx$
#这个积分是求不出来原函数的，但是我们可以将$e^{-x^2}$泰勒展开，然后再对得到的每个多项式积分就得到了一个和式：
#$\sum_{n=0}^{\infty}\frac{(-1)^nx^{2n+1}}{n!(2n+1)}$
#这个和式就是f(x)的泰勒展开式，我们用其前50项作为对f(x)的近似，并求出拟合函数。
from decimal import *
getcontext().prec = 100
maxn = 210
b = Decimal(1)
F = [0, 1]
for i in range(1, 50): #F为待求函数的泰勒展开式的系数，F[i]为x^i的系数，F中的项越多近似越好
    b *= -i
    F.append(0)
    F.append(1 / b / Decimal(2 * i + 1))
def asme(pos):
    length = pos + len(F) + 2
    t = [Decimal(0) for i in range(length)]
    for i in range(pos + 1):
        for j in range(len(F)):
            t[i + j] += a[pos][i] * F[j]
    res = Decimal(0)
    for i in range(length):
        res += t[i] * (pr[i + 1] - pl[i + 1]) / (i + 1)
    return res
L, R, T = Decimal(0), Decimal(2), 15 #待拟合区间为[L, R]，所用多项式的最高次数为T
a = [[Decimal(0) for j in range(maxn)] for i in range(maxn)]
c = [Decimal(0) for i in range(maxn)]
pl = [Decimal(1)]
pr = [Decimal(1)]
for i in range(maxn):
    pl.append(pl[-1] * L)
    pr.append(pr[-1] * R)
a[0][0] = 1 / (R - L).sqrt()
for i in range(1, T + 1):
    a[i][i] = Decimal(1)
    for j in range(i):
        sum = Decimal(0)
        for k in range(j + 1):
            sum += a[j][k] * (pr[i + k + 1] - pl[i + k + 1]) / (i + k + 1)
        for k in range(j + 1):
            a[i][k] -= sum * a[j][k]
    total = Decimal(0)
    for j in range(i + 1):
        for k in range(i + 1):
            total += a[i][j] * a[i][k] * (pr[j + k + 1] - pl[j + k + 1]) / (j + k + 1)
    length = total.sqrt()
    for j in range(i + 1):
        a[i][j] /= length
for pos in range(T + 1):
	res = asme(pos);
	for i in range(pos + 1):
		c[i] += a[pos][i] * res;
for i in range(T + 1):
    print("%.20e" % c[i], end=", ")
