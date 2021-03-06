% GAP Cirrone, November 2018
% This file read a .sdf file produced with PIC 3D simulation by 
% Dr Martina Zakova (ELI-Beamlines) and convert it in a csv file that can
% be read by the ELIMED Geant4 application
%
% IMPORTANT NOTICE: the laser propagatin direction is the X while in the
% ELIMED Geant application is Z. X and Z must be hence exchanged both in
% the momentum and in the position definition
%
fileToConvert = GetDataSDF('p0211.sdf')

c=299792458;
m_e=9.10938e-31;
e=1.60217662e-19;
eps0=8.85419e-12;
hmotnost_proton=1836.2;
hmotnost_carbon=22032;

% Momentum components as derived from the EPOCH code: they are not the
% cosine directions
%
px_p = fileToConvert.Particles.Px.subset_ionbackground.proton.data; 
py_p = fileToConvert.Particles.Py.subset_ionbackground.proton.data;
pz_p = fileToConvert.Particles.Pz.subset_ionbackground.proton.data;

% pz and px momenta are exchanged to agree with Geant4 ELIMED application
%
pzFinal = px_p;
%pxFinal = pz_p;
pxFinal = py_p; % ONLY FOR 2D PIC
pyFinal = py_p;


% Particle positions as derived from the EPOCH code
%
xx_p = fileToConvert.Particles.Px.subset_ionbackground.proton.grid.x; 
yy_p = fileToConvert.Particles.Py.subset_ionbackground.proton.grid.y;

% z and x coordinate are exchanged
%
zFinal = xx_p;
yFinal = yy_p;
xFinal = yy_p; %******** only for 2D PIC


% Now I select particles that have only a pz positive value
%
index = find(pzFinal > 0);
pzFinalSelected = pzFinal(index);
pxFinalSelected = pxFinal(index);
pyFinalSelected = pyFinal(index);

zFinalSelected = zFinal(index);
yFinalSelected = yFinal(index);

% pzFinalSelected, py... and px... ARE NOT cosine directions
% Cosine direction must be calculated from these components
%
pzFinalSelectedCosineDirection = pzFinalSelected./sqrt(pzFinalSelected.^2 + pyFinalSelected.^2 + pxFinalSelected.^2);
pyFinalSelectedCosineDirection = pyFinalSelected./sqrt(pzFinalSelected.^2 + pyFinalSelected.^2 + pxFinalSelected.^2);
pxFinalSelectedCosineDirection = pxFinalSelected./sqrt(pzFinalSelected.^2 + pyFinalSelected.^2 + pxFinalSelected.^2);

% A normalisation is then performed that is probably not necessary for the
% Geant4 input
%
px_p1=px_p./(m_e*hmotnost_proton*c);
py_p1=py_p./(m_e*hmotnost_proton*c);
pz_p1=pz_p./(m_e*hmotnost_proton*c);

% Derivation of the relativistic gamma factor and of the particle energy
%
gamma_p=sqrt(1.0+px_p1.^2+py_p1.^2+pz_p1.^2);
energy_p=0.511*hmotnost_proton*(gamma_p-1.0);

% Also energy must be selected for particles with positive momentum
%
energyFinalSelected = energy_p(index);

% Creo i vettori mancanti (zz_p, time e pdg)
%
pdg = linspace(2212,2212,length(pxFinalSelected));
pdg = pdg';
time = linspace(0,0, length(pxFinalSelected));
time = time';
xFinalSelected = linspace(0,0,length(pxFinalSelected));
xFinalSelected = xFinalSelected';
    
% Scrivo un file csv
%
% Prima creo la matrice
header = {'pz', 'py', 'px', 'z', 'y', 'x', 'ekin', 'pdg', 't'}'; 
M = [pzFinalSelectedCosineDirection, pyFinalSelectedCosineDirection, pxFinalSelectedCosineDirection, zFinalSelected, yFinalSelected, xFinalSelected, energyFinalSelected, pdg, time];
Mheaders = [M;header];

% Poi scrivo la matrice
%
csvwrite_with_headers('convertedFile.csv', M, header);