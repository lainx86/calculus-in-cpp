#include <fmt/core.h>
#include <cmath>
#include <vector>

double limit(double (*f)(double), double a, double e = 1e-12)
{
    double limit_kiri, limit_kanan;

    std::vector<double> x_kiri, x_kanan;

    fmt::print("Menghitung lim(x→{}) f(x)\n\n", a);

    // Pendekatan dari Kiri (Zeno's approach)
    double current = a - 5.0; // Mulai dari agak jauh
    while (std::abs(a - current) > e && x_kiri.size() < 25)
    {
        x_kiri.push_back(current);
        current += (a - current) * 0.5; // Lompat setengah jarak terus menerus
    }

    // Pendekatan dari Kanan
    current = a + 5.0;
    while (std::abs(current - a) > e && x_kanan.size() < 25)
    {
        x_kanan.push_back(current);
        current -= (current - a) * 0.5;
    }

    // Output Pendekatan dari Kiri
    fmt::print("📍 PENDEKATAN DARI KIRI (x < {})\n", a);
    fmt::print("{:<5} | {:<18} | {:<18}\n", "Step", "x", "f(x)");
    fmt::print("{:-<48}\n", "");

    for (size_t i = 0; i < x_kiri.size(); ++i)
    {
        double x = x_kiri[i];
        double fx = f(x);
        fmt::print("{:<5} | {:>16.12f} | {:>16.12f}\n", i + 1, x, fx);
        limit_kiri = fx;
    }

    // Output Pendekatan dari Kanan
    fmt::print("\nPENDEKATAN DARI KANAN (x > {})\n", a);
    fmt::print("{:<5} | {:<18} | {:<18}\n", "Step", "x", "f(x)");
    fmt::print("{:-<48}\n", "");

    for (size_t i = 0; i < x_kanan.size(); ++i)
    {
        double x = x_kanan[i];
        double fx = f(x);
        fmt::print("{:<5} | {:>16.12f} | {:>16.12f}\n", i + 1, x, fx);
        limit_kanan = fx;
    }

    // Verifikasi Limit
    fmt::print("\n{:=<48}\n", "");

    double diff = std::abs(limit_kiri - limit_kanan);
    fmt::print("Limit dari kiri  : {:.12f}\n", limit_kiri);
    fmt::print("Limit dari kanan : {:.12f}\n", limit_kanan);
    fmt::print("Selisih          : {:.2e}\n", diff);

    if (diff < 1e-3)
    {
        double avg_limit = (limit_kiri + limit_kanan) / 2.0;
        fmt::print("\nLIMIT DITEMUKAN!\n");
        fmt::print("lim(x→{}) f(x) = {:.12f}\n", a, avg_limit);
        fmt::print("Nilai eksak    = 3.000000000000\n");
        fmt::print("Error          = {:.2e}\n", std::abs(avg_limit - 3.0));
        return avg_limit;
    }
    else
    {
        fmt::print("\nLIMIT TIDAK ADA\n");
        fmt::print("  (Divergen atau Jump Discontinuity)\n");
        return NAN;
    }
}

double f(double x)
{
    // f(x) = (x^3 - 1) / (x-1)
    return (std::pow(x, 2) - 4) / (x - 2);
}

int main()
{
    limit(f, 2);
    return 0;
}
