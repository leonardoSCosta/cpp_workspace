import sympy
from sympy import Matrix, init_printing, pprint, sqrt, latex
init_printing(use_unicode=True)

uax, uay, ubx, uby = sympy.symbols('u_{ax}, u_{ay}, u_{bx}, u_{by}')  # Control input
xax, xay, xbx, xby = sympy.symbols('x_{ax}, x_{ay}, x_{bx}, x_{by}')  # States
xoax, xoay, xobx, xoby = sympy.symbols('x_{oax}, x_{oay}, x_{obx}, x_{oby}')  # Obstacle states
voax, voay, vobx, voby = sympy.symbols('v_{oax}, v_{oay}, v_{obx}, v_{oby} ')  # Obstacle velocity
wax, way, wbx, wby = sympy.symbols('w_{ax}, w_{ay}, w_{bx}, w_{by}')  # State noises
vax, vay, vbx, vby = sympy.symbols('v_{ax}, v_{ay}, v_{bx}, v_{by} ')  # Input noises
woax, woay, wobx, woby = sympy.symbols('w_{oax}, w_{oay}, w_{obx}, w_{oby}')  # State noises
voax, voay, vobx, voby = sympy.symbols('v_{oax}, v_{oay}, v_{obx}, v_{oby} ')  # Input noises
eijx, eijy, eikx, eiky = sympy.symbols('e_{ijx}, e_{ijy}, e_{ikx}, e_{iky}')  # e
Rij, Rik, gamma, P, sigma = sympy.symbols('R_{ij}, R_{ik}, gamma, P, sigma')  # Constants


def nchoosek(n, k):
    prevSolution = n - k + 1
    solution = 0
    for i in range(1, k):
        solution = prevSolution * (n - k + 1 + i) / (i + 1)
        prevSolution = solution
    return prevSolution


if __name__ == "__main__":
    N = 2
    Nobs = 2

    u = Matrix([[uax, ubx], [uay, uby]])
    x = Matrix([[xax, xbx], [xay, xby]])
    xoa = Matrix([[xoax, xobx], [xoay, xoby]])
    voa = Matrix([[voax, vobx], [voay, voby]])
    w = Matrix([[wax, wbx], [way, wby]])
    v = Matrix([[vax, vbx], [vay, vby]])
    wo = Matrix([[woax, wobx], [woay, woby]])
    vo = Matrix([[voax, vobx], [voay, voby]])

    print("u")
    print(latex(u, mat_delim='', mat_str='bmatrix', mode='plain'))
    print("x")
    print(latex(x, mat_delim='', mat_str='bmatrix', mode='plain'))
    print("xoa")
    print(latex(xoa, mat_delim='', mat_str='bmatrix', mode='plain'))
    print("voa")
    print(latex(voa, mat_delim='', mat_str='bmatrix', mode='plain'))
    print("w")
    print(latex(w, mat_delim='', mat_str='bmatrix', mode='plain'))
    print("v")
    print(latex(v, mat_delim='', mat_str='bmatrix', mode='plain'))
    print("wo")
    print(latex(wo, mat_delim='', mat_str='bmatrix', mode='plain'))
    print("vo")
    print(latex(vo, mat_delim='', mat_str='bmatrix', mode='plain'))

    constraints = N - 1
    constraints = constraints + Nobs * N

    A = Matrix.zeros(constraints, 2 * N)
    b = Matrix.zeros(constraints, 1)

    count = 0

    # Robot loop
    for i in range(0, N - 1):
        for j in range(i+1, N):
            maxDVijx = abs((v.col(i) + v.col(j))[0])
            maxDVijy = abs((v.col(i) + v.col(j))[1])

            maxDXijx = abs((x.col(i) - x.col(j))
                           [0]) + abs((w.col(i) + w.col(j))[0])
            maxDXijy = abs((x.col(i) - x.col(j))
                           [1]) + abs((w.col(i) + w.col(j))[1])

            eVec = Matrix([[eijx], [eijy]])
            print("evec ij")
            print(latex(eVec, mat_delim='', mat_str='bmatrix', mode='plain'))

            A[count, 2 * i: 2 * i + 2] = -2 * eVec.transpose()
            A[count, 2 * j: 2 * j + 2] = 2 * eVec.transpose()
            maxDVij = Matrix([[maxDVijx], [maxDVijy]])
            maxDXij = Matrix([[maxDXijx], [maxDXijy]])

            absMaxDVij = maxDVij.__abs__()
            absMaxDXij = maxDXij.__abs__()

            h1 = eVec[0]**2 - Rij**2 - 2 * \
                absMaxDVij[0] * absMaxDXij[0] / gamma
            h2 = eVec[1]**2 - Rij**2 - 2 * \
                absMaxDVij[1] * absMaxDXij[1] / gamma

            b[count] = gamma * (h1 + h2)**P
            count += 1

    # Obstacle loop
    for i in range(0, N):
        for j in range(0, Nobs):
            maxDVijx = abs((v.col(i) + vo.col(j))[0])
            maxDVijy = abs((v.col(i) + vo.col(j))[1])

            maxDXijx = abs((x.col(i) - xoa.col(j))
                           [0]) + abs((w.col(i) + wo.col(j))[0])
            maxDXijy = abs((x.col(i) - xoa.col(j))
                           [1]) + abs((w.col(i) + wo.col(j))[1])

            eVec = Matrix([[eikx], [eiky]])
            print("evec ik")
            print(latex(eVec, mat_delim='', mat_str='bmatrix', mode='plain'))

            A[count, 2 * i: 2 * i + 2] = -2 * eVec.transpose()
            A[count, 2 * j: 2 * j + 2] = 2 * eVec.transpose()
            maxDVij = Matrix([[maxDVijx], [maxDVijy]])
            maxDXij = Matrix([[maxDXijx], [maxDXijy]])

            absMaxDVij = maxDVij.__abs__()
            absMaxDXij = maxDXij.__abs__()

            h1 = eVec[0]**2 - Rik**2 - 2 * absMaxDVij[0] * \
                absMaxDXij[0] / gamma - 2 * eVec[0] * voa.col(j)[0] / gamma
            h2 = eVec[1]**2 - Rik**2 - 2 * absMaxDVij[1] * \
                absMaxDXij[1] / gamma - 2 * eVec[1] * voa.col(j)[1] / gamma

            b[count] = gamma * (h1 + h2)**P
            count += 1

    vhat = u.transpose().reshape(2 * N, 1)
    f = -2 * vhat
    H = 2 * Matrix.eye(2 * N)

    print("A")
    print(latex(A, mat_delim='', mat_str='bmatrix', mode='plain'))
    print("H")
    print(latex(H, mat_delim='', mat_str='bmatrix', mode='plain'))
    print("f")
    print(latex(f, mat_delim='', mat_str='bmatrix', mode='plain'))
    print("b")
    print(latex(b, mat_delim='', mat_str='bmatrix', mode='plain'))
