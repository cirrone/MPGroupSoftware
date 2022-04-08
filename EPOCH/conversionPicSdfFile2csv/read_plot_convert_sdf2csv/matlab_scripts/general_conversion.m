% This script read a .sdf file produced with EPOCH simulation and convert it in a .csv file. 
% Then it plot the phase space, both non-normalized and normalized, density profile and so on.

% READING THE .sdf FILE USING THE GetDataSDF FUNCTION
structure = GetDataSDF('/home/beatrice/epoch/epoch2d/laser_focus/Data/0008.sdf')

%%
% Extraction of the variables of interest

% EDIT THIS SECTION DEPENDING ON THE SIMULATION

% px = structure.Particles.Px.Right.data;
% py = structure.Particles.Px.Left.data;

% x = structure.Grid.Grid.x;
% y = structure.Grid.Grid.y;

number_density_e=structure.Derived.Number_Density.electron.data;
number_density_p=structure.Derived.Number_Density.proton.data;

% electric_field_y=structure.Electric_Field.Ey.data;

%x_px_L=structure.dist_fn.x_px.Left.data;
%x_px_R=structure.dist_fn.x_px.Right.data;

%%
% PLOT THE PHASE-SPACE: non-normalized

% EDIT THIS SECTION DEPENDING ON THE SIMULATION

% figure;
% scatter(y,py,'.','DisplayName','Leftward beam')
% hold on
% scatter(x,px,'.','DisplayName','Rightward beam')
% hold off
% xlabel('Particle.Px.Grid.x [m]','FontSize',10);
% ylabel('Particle.Px.Data [kg x m/s]','FontSize',10);


%%
% PLOT THE PHASE-SPACE: normalized to the mass of the electron

% EDIT THIS SECTION DEPENDING ON THE SIMULATION

% % Set costants
% c=299792458; % speed of light
% m_e=9.10938e-31; % electron mass in kg
% e=1.60217662e-19; % electron charge in C
% eps0=8.85419e-12; % permittivity in vacuum in F/m
% %m_proton=1836.2; % proton mass in unit of electron masses
% %m_carbon=22032; % carbon mass in unit of electron masses
% 
% % Normalization
% px_R_N=(px-min(px))./(max(px)-min(px));
% px_L_N=(py-min(py))./(max(py)-min(py));
% x_R_N=x./max(x);
% x_L_N=y./max(y);
% 
% % Plot
% figure;
% scatter(x_L_N,px_L_N,'.','DisplayName','Normalized leftward beam')
% hold on
% scatter(x_R_N,px_R_N,'.','DisplayName','Normalized rightward beam')
% hold off
% xlabel('Normalized grid position [m]','FontSize',10);
% ylabel('Normalized momentum [kg x m/s]','FontSize',10);

%%
% PLOT 2D SPACE FROM dist_fn BLOCK

% figure;
% surf(x_px_L)
% figure;
% surf(x_px_R)

%%
% PLOT DENSITY PROFILE

% EDIT THIS SECTION DEPENDING ON THE SIMULATION

figure;
surf(number_density_p)
figure;
surf(number_density_e)

%%
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