#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#define MAXN 1005      // max müşteri sayısı + depo için güvenli sınır
#define MAXM 15        // max araç sayısı (m <= 10)
#define MAXSAV (1000000 + 5) // max savings çifti (n<=1000 için ~500k)

// -------------------- GLOBAL DEĞİŞKENLER --------------------

int nClients;          // müşteri sayısı (n)
int m;                 // araç sayısı
int k;                 // her aracın max müşteri kapasitesi

double X[MAXN], Y[MAXN];          // 0: depo, 1..n: müşteriler
double distMat[MAXN][MAXN];       // mesafe matrisi

// VRP rotaları için yapı
typedef struct {
    int size;                  // rota içindeki müşteri sayısı
    int cust[MAXN];            // müşteri indeksleri (1..n)
} Route;

Route routes[MAXN];            // en başta n rota olabilir (her müşteri için bir)
int routeCount;                // aktif rota sayısı
int belong[MAXN];              // belong[i] = i. müşteri hangi rotada

// Savings çifti
typedef struct {
    int a, b;
    double s;
} Saving;

Saving S[MAXSAV];
int sCount = 0;

// -------------------- MESAFE HESABI --------------------

double distance2(int a, int b) {
    double dx = X[a] - X[b];
    double dy = Y[a] - Y[b];
    return sqrt(dx*dx + dy*dy);
}

void build_dist() {
    for (int i = 0; i <= nClients; i++) {
        for (int j = 0; j <= nClients; j++) {
            distMat[i][j] = distance2(i, j);
        }
    }
}

// -------------------- ROTA MALİYETİ --------------------
// Rota: depo -> cust[0] -> ... -> cust[size-1] -> depo

double route_cost(int *r, int size) {
    if (size == 0) return 0.0;
    double c = 0.0;
    c += distMat[0][r[0]];
    for (int i = 0; i < size - 1; i++) {
        c += distMat[r[i]][r[i+1]];
    }
    c += distMat[r[size-1]][0];
    return c;
}

// -------------------- 2-OPT (LOCAL SEARCH) --------------------

void two_opt(int *r, int size) {
    if (size < 3) return;

    int temp[MAXN];
    int improved = 1;

    while (improved) {
        improved = 0;
        double best = route_cost(r, size);

        for (int i = 0; i < size - 1; i++) {
            for (int j = i + 1; j < size; j++) {

                // rotayı temp'e kopyala
                for (int t = 0; t < size; t++)
                    temp[t] = r[t];

                // i..j arasını ters çevir
                int a = i, b = j;
                while (a < b) {
                    int tmp = temp[a];
                    temp[a] = temp[b];
                    temp[b] = tmp;
                    a++; b--;
                }

                double newCost = route_cost(temp, size);
                if (newCost + 1e-9 < best) {
                    for (int t = 0; t < size; t++)
                        r[t] = temp[t];
                    best = newCost;
                    improved = 1;
                }
            }
        }
    }
}

// -------------------- NEAREST NEIGHBOUR (TSP İÇİN) --------------------

int nn_route[MAXN];
int used[MAXN];

void nearest_neighbor() {
    for (int i = 0; i <= nClients; i++) used[i] = 0;

    int last = 0; // depo
    used[0] = 1;

    for (int step = 0; step < nClients; step++) {
        double best = DBL_MAX;
        int bestj = -1;
        for (int j = 1; j <= nClients; j++) {
            if (!used[j] && distMat[last][j] < best) {
                best = distMat[last][j];
                bestj = j;
            }
        }
        nn_route[step] = bestj;
        used[bestj] = 1;
        last = bestj;
    }
}

// -------------------- SAVINGS VRP --------------------

int cmpSaving(const void *A, const void *B) {
    const Saving *x = (const Saving*)A;
    const Saving *y = (const Saving*)B;
    if (x->s < y->s) return 1;
    if (x->s > y->s) return -1;
    return 0;
}

// Bir rotayı ters çevir (rota içindeki müşteri sırasını)
void reverse_route(Route *r) {
    int i = 0, j = r->size - 1;
    while (i < j) {
        int tmp = r->cust[i];
        r->cust[i] = r->cust[j];
        r->cust[j] = tmp;
        i++; j--;
    }
}

// routes[rb] rotasını sil, yerine son rotayı koy
void remove_route(int rb) {
    if (rb < 0 || rb >= routeCount) return;
    int last = routeCount - 1;
    if (rb != last) {
        // son rotayı rb'nin yerine taşı
        routes[rb] = routes[last];
        // belong[] güncelle
        for (int i = 0; i < routes[rb].size; i++) {
            int c = routes[rb].cust[i];
            belong[c] = rb;
        }
    }
    routeCount--;
}

// Clarke–Wright Savings algoritması
void savings_vrp() {
    // 1) Başlangıç: her müşteri kendi rotasında
    routeCount = nClients;
    for (int i = 0; i < routeCount; i++) {
        routes[i].size = 1;
        routes[i].cust[0] = i + 1; // müşteri indexi: 1..n
        belong[i + 1] = i;
    }

    // 2) Savings hesapla
    sCount = 0;
    for (int i = 1; i <= nClients; i++) {
        for (int j = i + 1; j <= nClients; j++) {
            double s = distMat[0][i] + distMat[0][j] - distMat[i][j];
            S[sCount].a = i;
            S[sCount].b = j;
            S[sCount].s = s;
            sCount++;
        }
    }

    // 3) Savings büyükten küçüğe sırala
    qsort(S, sCount, sizeof(Saving), cmpSaving);

    // 4) Savings'e göre rotaları birleştir
    for (int idx = 0; idx < sCount && routeCount > m; idx++) {
        int a = S[idx].a;
        int b = S[idx].b;

        int ra = belong[a];
        int rb = belong[b];

        if (ra == rb) continue; // aynı rotadaysa birleştirme

        Route *RA = &routes[ra];
        Route *RB = &routes[rb];

        // kapasite kontrolü
        if (RA->size + RB->size > k) continue;

        int merged = 0;

        // 4 olası uç kombinasyonu:
        // 1) RA sonu a, RB başı b -> RA + RB
        if (!merged && RA->cust[RA->size - 1] == a && RB->cust[0] == b) {
            int pos = RA->size;
            for (int i = 0; i < RB->size; i++) {
                RA->cust[pos++] = RB->cust[i];
                belong[RB->cust[i]] = ra;
            }
            RA->size = pos;
            remove_route(rb);
            merged = 1;
        }
            // 2) RA başı a, RB sonu b -> RB + RA
        else if (!merged && RA->cust[0] == a && RB->cust[RB->size - 1] == b) {
            // RB'nin sonuna RA'yı ekle
            int pos = RB->size;
            for (int i = 0; i < RA->size; i++) {
                RB->cust[pos++] = RA->cust[i];
                belong[RA->cust[i]] = rb;
            }
            RB->size = pos;
            remove_route(ra);
            merged = 1;
        }
            // 3) RA sonu a, RB sonu b -> RB'yi ters çevir, sonra RA + RB
        else if (!merged && RA->cust[RA->size - 1] == a && RB->cust[RB->size - 1] == b) {
            reverse_route(RB);
            int pos = RA->size;
            for (int i = 0; i < RB->size; i++) {
                RA->cust[pos++] = RB->cust[i];
                belong[RB->cust[i]] = ra;
            }
            RA->size = pos;
            remove_route(rb);
            merged = 1;
        }
            // 4) RA başı a, RB başı b -> RA'yı ters çevir, sonra RA + RB
        else if (!merged && RA->cust[0] == a && RB->cust[0] == b) {
            reverse_route(RA);
            int pos = RA->size;
            for (int i = 0; i < RB->size; i++) {
                RA->cust[pos++] = RB->cust[i];
                belong[RB->cust[i]] = ra;
            }
            RA->size = pos;
            remove_route(rb);
            merged = 1;
        }

        // routeCount m'ye düştüyse daha fazla birleştirmeye gerek yok
        if (routeCount <= m) break;
    }
}

// -------------------- MAIN --------------------

int main() {
    // Giriş: n m k
    if (scanf("%d %d %d", &nClients, &m, &k) != 3) {
        return 0;
    }

    // Müşteri koordinatları (1..n)
    for (int i = 1; i <= nClients; i++) {
        scanf("%lf %lf", &X[i], &Y[i]);
    }

    // Depo koordinatı (0)
    scanf("%lf %lf", &X[0], &Y[0]);

    // Mesafe matrisi
    build_dist();

    // ---------------- TEK ARAÇ DURUMU (TSP) ----------------
    if (m == 1) {
        nearest_neighbor();
        two_opt(nn_route, nClients);

        double cost = route_cost(nn_route, nClients);
        printf("%.6f\n", cost);

        printf("0 ");
        for (int i = 0; i < nClients; i++) {
            printf("%d ", nn_route[i]);
        }
        printf("0\n");

        return 0;
    }

    // ---------------- ÇOK ARAÇ DURUMU (VRP) ----------------
    savings_vrp();

    double total = 0.0;

    // Her rota için 2-opt ve maliyet
    for (int r = 0; r < routeCount; r++) {
        two_opt(routes[r].cust, routes[r].size);
        total += route_cost(routes[r].cust, routes[r].size);
    }

    printf("%.6f\n", total);

    // Rotaları yazdır
    for (int r = 0; r < routeCount; r++) {
        printf("0 ");
        for (int i = 0; i < routes[r].size; i++) {
            printf("%d ", routes[r].cust[i]);
        }
        printf("0\n");
    }

    return 0;
}
