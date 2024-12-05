#include <TGraph.h>
#include <TRandom3.h>
#include <TCanvas.h>
#include <iostream>
#include <iomanip>
#include <cmath>

void simulate_and_fix_no_errors() {
    // Parametri del modello
    double I0 = 1e-5;     // Corrente di saturazione (in mA)
    double Eta_VT = 25.;  // Fattore di idealità moltiplicato per V_T (in mV)

    // Nuovi dati
    const int n_new = 10;
    double x_new[n_new], y_new[n_new];
    TRandom3 rand(0);

    for (int i = 0; i < n_new; i++) {
        // Genera nuovi x casuali nell'intervallo (in mV)
        x_new[i] = rand.Uniform(50, 300);

        // Calcola y basato sul modello (senza errori)
        y_new[i] = I0 * (exp(x_new[i] / Eta_VT) - 1); // Corrente in mA
    }

    // Stampa i nuovi dati
    std::cout << "Dati simulati (senza errori):\n";
    std::cout << std::setw(10) << "x (mV)" << std::setw(15) << "y (mA)\n";
    for (int i = 0; i < n_new; i++) {
        std::cout << std::setw(10) << x_new[i] << std::setw(15) << y_new[i] << "\n";
    }

    // Grafico
    TCanvas *c1 = new TCanvas("c1", "Dati Simulati", 800, 600);
    TGraph *gr = new TGraph(n_new, x_new, y_new);
    gr->SetTitle("Dati Simulati;Voltage (mV);Current (mA)");
    gr->SetMarkerStyle(21);
    gr->SetMarkerColor(kBlue);
    gr->Draw("AP");
}
