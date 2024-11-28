#include <TGraphErrors.h>
#include <TF1.h>
#include <TCanvas.h>
#include <iostream>
#include "TStyle.h"
#include "TAxis.h"

void fit_caratteristica() {
// define data
// valori della ddp

//Silicio
/*float_t x[10]={400,440,480,500,520,560,600,650,700,800};
float_t y[10]={0.02,0.04,0.07,0.09,0.16,0.26,0.85,2.32,8.93,30.69};
float_t ex[10]={15,16.5,17.5,18,18.5,19.6,20.6,21.9,29,31.2}; // errori oscill
float_t ey[10]={0.03,0.03,0.03,0.03,0.03,0.03,0.03,0.05,0.11,0.33}; // e degli errori multim*/

//Germanio
float_t x[10]={50,100,120,150,180,200,250,300,350,400};
float_t y[10]={0.01,0.03,0.05,0.11,0.22,0.31,0.81,1.83,3.69,6.53};
float_t ex[10]={2.5,3.6,6.2,6.7,7.4,7.8,9,10.3,14.5,15.6}; // errori oscill
float_t ey[10]={0.001,0.001,0.001,0.001,0.03,0.03,0.04,0.05,0.07,0.1}; // e degli errori multim

TCanvas *c1 = new TCanvas("c1", "Canvas 1", 800, 600);


// create graph
TGraphErrors *gr = new TGraphErrors(10,x,y,ex,ey);
gr->SetTitle("Diodo al Si");

// draw with options
gr->SetMarkerStyle(4);
gr->Draw("AP");
c1->SetLogy();
c1->SetGrid();
gr->GetXaxis()->SetTitle("Voltage (mV)");
gr->GetYaxis()->SetTitle("Current (mA)");

// define user fit function and fit
TF1 *f1 = new TF1("f1","[0]*(exp(x/[1])-1)",150,400); //cambiare per Si-Ge
/*f1->SetParameter(0,1e-6);    // necessario per user defined function
f1->SetParameter(1,50.);     // necessario per user defined function*/
f1->SetParameter(0, 1e-6); // Stima iniziale per I0
f1->SetParameter(1, 25.); // Stima iniziale per Eta*VT
f1->SetParName(0,"I0");
f1->SetParName(1,"Eta*VT");
gr->Fit("f1","R");
gStyle->SetOptFit(1111);
}
