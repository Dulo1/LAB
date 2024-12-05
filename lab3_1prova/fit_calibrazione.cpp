#include <TGraphErrors.h>
#include <TF1.h>
#include <TCanvas.h>
#include <iostream>
#include "TStyle.h"
#include "TAxis.h"

void fit_calibrazione() {
    // Definizione dei dati: valori di V (multimetro) e V (oscilloscopio) con errori
    Float_t x[8] = { 100.5, 205.6,307.5, 401.4, 502.6, 602.2, 713, 825};  //Inserisci qui i valori di V_multimetro
    Float_t y[8] = { 100, 200, 300, 400, 500, 600, 700, 800}; // Inserisci qui i valori di V_oscilloscopio
    Float_t ex[8] = {0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 5.6, 5.6}; // Errori su V_multimetro
    Float_t ey[8] = {3.6, 7.8, 10.3, 15, 18, 20.6, 29, 31.2};// Inserisci qui gli errori su V_oscilloscopio

    // Creazione del grafico con errori
    TGraphErrors *gr = new TGraphErrors(8, x, y, ex, ey);
    gr->SetTitle("Calibrazione Oscilloscopio vs Multimetro;V_{Multimetro} (V);V_{Oscilloscopio} (V)");
    gr->SetMarkerStyle(20);
    gr->SetMarkerColor(kBlue);

    // Disegno del grafico
    TCanvas *c1 = new TCanvas("c1", "Calibrazione", 600, 800);
    gr->Draw("AP");

    // Definizione della funzione di fit lineare
    TF1 *f1 = new TF1("f1", "[0] + [1]*x", 0, 1000); // a + b*x
    f1->SetParameter(0, 0);  // Stima iniziale per l'intercetta a
    f1->SetParameter(0, 0);  // Stima iniziale per la pendenza b
    f1->SetParName(0, "Offset");  // Nome del parametro a
    f1->SetParName(1, "p1");  // Nome del parametro b

    // Esecuzione del fit
    gr->Fit(f1, "R"); // Fit nell'intervallo specificato
    gStyle->SetOptFit(1111);

    // Visualizzazione dei risultati del fit
    c1->SetGrid();
    c1->Update();

    // Stampa dei risultati
    std::cout << "Risultati del fit di calibrazione:" << std::endl;
    std::cout << "a (intercetta) = " << f1->GetParameter(0) << " +/- " << f1->GetParError(0) << std::endl;
    std::cout << "b (pendenza) = " << f1->GetParameter(1) << " +/- " << f1->GetParError(1) << std::endl;
}
/*#include <iostream>
#include <fstream>
#include <vector>
#include "TCanvas.h"
#include "TGraph.h"
#include "TF1.h"
#include "TApplication.h"
#include "TFile.h"

void fit_data() {

    const char* filename = "datical.txt";  

    
    std::vector<double> V_oscilloscope;
    std::vector<double> V_multimeter;

    // Lettura dei dati dal file
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Errore nell'apertura del file " << filename << std::endl;
        return;
    }

    double y, x;
    while (infile >> y >> x) {
        V_oscilloscope.push_back(y);  // Prima colonna: V(oscilloscopio)
        V_multimeter.push_back(x);    // Seconda colonna: V(multimetro)
    }
    infile.close();

    int n = V_oscilloscope.size();
    if (n == 0) {
        std::cerr << "Il file è vuoto o non contiene dati validi." << std::endl;
        return;
    }
    // Creazione del grafico
    TGraph *graph = new TGraph(n, &V_multimeter[0], &V_oscilloscope[0]);
    graph->SetTitle("V(oscilloscopio) vs V(multimetro);V(multimetro);V(oscilloscopio)"); //sono in mV!!
    graph->SetMarkerStyle(20);

    // Definizione della funzione di fit lineare
    TF1 *fitFunc = new TF1("fitFunc", "[0] + [1]*x", 0, 1000    ); 
    fitFunc->SetParameters(0, 1); // stima iniziale di a e b

    // Esecuzione del fit
    graph->Fit("fitFunc", "Q"); // Opzione "Q" per un fit silenzioso

    // Creazione di un canvas per il grafico
    TCanvas *c1 = new TCanvas("c1", "Fit dei dati", 800, 600);
    graph->Draw("AP"); // "AP" per disegnare i punti e l'asse

    // Estrazione dei parametri del fit
    double a = fitFunc->GetParameter(0);
    double b = fitFunc->GetParameter(1);

    std::cout << "Valore di a: " << a << std::endl;
    std::cout << "Valore di b: " << b << std::endl;
    double error_a = fitFunc->GetParError(0);
    double error_b = fitFunc->GetParError(1);

    std::cout << "Errore su a: " << error_a << std::endl;
    std::cout << "Errore su b: " << error_b << std::endl;

    // Salvare il canvas come file immagine (opzionale)
    c1->SaveAs("fit_result.png");
}*/