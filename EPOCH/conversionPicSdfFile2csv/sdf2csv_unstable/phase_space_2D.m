% This script read a .sdf file produced with EPOCH simulation and convert it in a .csv file. 
% Then it plot the phase space, both non-normalized and normalized.

% READING THE .sdf FILE USING THE GetDataSDF FUNCTION
structure = GetDataSDF('/home/beatrice/epoch/epoch1d/Data/2fasci-carico-neutro/0090.sdf')

%%
% Extraction of the variables of interest: Momentum components as derived from the EPOCH code.
% They are not the cosine directions.

% EDIT THIS SECTION DEPENDING ON THE SIMULATION

px_R = structure.Particles.Px.Right.data;
px_L = structure.Particles.Px.Left.data;

%%
% Extraction of the variables of interest: Particle positions as derived from the EPOCH code.

% EDIT THIS SECTION DEPENDING ON THE SIMULATION

x_R = structure.Particles.Px.Right.grid.x; 
x_L = structure.Particles.Px.Left.grid.x;

%%
% PLOT THE PHASE-SPACE: non-normalized

figure;
scatter(x_L,px_L,'.','DisplayName','Leftward beam')
hold on
scatter(x_R,px_R,'.','DisplayName','Rightward beam')
hold off
xlabel('Particle.Px.Grid.x [m]','FontSize',10);
ylabel('Particle.Px.Data [kg x m/s]','FontSize',10);


%%
% PLOT THE PHASE-SPACE: normalized to the mass of the electron

% Set costants
c=299792458; % speed of light
m_e=9.10938e-31; % electron mass in kg
e=1.60217662e-19; % electron charge in C
eps0=8.85419e-12; % permittivity in vacuum in F/m
%m_proton=1836.2; % proton mass in unit of electron masses
%m_carbon=22032; % carbon mass in unit of electron masses

% Normalization
px_R_N=(px_R-min(px_R))./(max(px_R)-min(px_R));
px_L_N=(px_L-min(px_L))./(max(px_L)-min(px_L));
x_R_N=x_R./max(x_R);
x_L_N=x_L./max(x_L);

% Plot
figure;
scatter(x_L_N,px_L_N,'.','DisplayName','Normalized leftward beam')
hold on
scatter(x_R_N,px_R_N,'.','DisplayName','Normalized rightward beam')
hold off
xlabel('Normalized grid position [m]','FontSize',10);
ylabel('Normalized momentum [kg x m/s]','FontSize',10);

% %%
% % PLOT PHASE-SPACE FROM DISTRIBUTION FUNCTION
% 
% x_px_L=structure.dist_fn.x_px.Left.data;
% x_px_R=structure.dist_fn.x_px.Right.data;
% figure;
% surf(x_px_R)
% figure;
% surf(x_px_L)

% %%
% % WRITING THE .csv FILE
% 
% % EDIT THIS SECTION DEPENDING ON THE SIMULATION
% 
% % First: create the matrix
% header = {'px_R', 'px_L', 'x_R', 'x_L'}'; 
% M = [px_R, px_L, x_R, x_L];
% Mheaders = [M;header];
% 
% % Second: write the matrix
% csvwrite_with_headers('/home/beatrice/GitHub/EPOCH/conversionPicSdfFile2csv/sdf2csv_unstable/2_stream_instability_filter_PHASE-SPACE_0094.csv', M, header);

%%
% CLEAR ALL THE VARIABLES
clear all