% Read the cvs file from PIC simulations
%
clear all
close all

fileCaricato = load('data_p_SelectedInEnergy_30MeV.csv');
%% 
% Per leggere l'output dei PIC
pz = fileCaricato(:,1);
py = fileCaricato(:,2);
px = fileCaricato(:,3);

z = fileCaricato(:,4);
y = fileCaricato(:,5);
x = fileCaricato(:,6);

energy = fileCaricato(:,7);
clear fileCaricato

%% 
% Per leggere l'output da Geant4
pz = fileCaricato(:,9);
py = fileCaricato(:,10);
px = fileCaricato(:,11);

z = fileCaricato(:,5);
y = fileCaricato(:,6);
x = fileCaricato(:,7);

energy = fileCaricato(:,12);
clear fileCaricato

%%
% Rotation of the direction cosines in angles around the y axis and 
% of the x-axis of the Geant4 reference frame 
%
% Rotation along y
%
rotationAngleAlong_yDeg = 0; % in deg
rotationAngleAlong_yRad = degtorad(rotationAngleAlong_yDeg); % in deg

pzRotated = pz*cos(rotationAngleAlong_yRad) + px*sin(rotationAngleAlong_yRad);
pyRotated = py;
pxRotated = px*cos(rotationAngleAlong_yRad) - pz*sin(rotationAngleAlong_yRad);

% Rotation along x (of the Geant4 frame)
%
rotationAngleAlong_xDeg = 0; % in deg
rotationAngleAlong_xRad = degtorad(rotationAngleAlong_xDeg); % in deg

pzRotated_2 = pzRotated*cos(rotationAngleAlong_xRad) - pyRothist(pz, 100);
ated*sin(rhist(theta_1Deg, 10)
xlabel('Deg')
title('Theta 1 (pz)')otationAngleAlong_xRad); 
pyRotated_2 = pzRotated*sin(rotationAngleAlong_xRad) + pyRotated*cos(rotationAngleAlong_xRad);
pxRotated_2 = pxRotated;


%%
pdg = linspace(2212,2212,length(z));
pdg = pdg';

time = linspace(0,0, length(z));
time = time';

%%
 px = normrnd(0, 0.06,[1 length(z)]);
 px = px';

% 
% py = normrnd(0, 0.1,[1 length(z)]);
% py = py';




%%
%hist(pz, 100);

% angles in radiants
%
theta_1 = acos(pz);
theta_2 = acos(px);
theta_3 = acos(py);

% angles in degrees
%
theta_1Deg = radtodeg(theta_1);
theta_2Deg = radtodeg(theta_2);
theta_3Deg = radtodeg(theta_3);
%%
% In case you want create directly a Theta_1 with a gaussian distribution
%
theta_1Deg  = normrnd(0, 1,[1 length(z)]);
theta_1Rad = degtorad(theta_1Deg);
pz = cos(theta_1Rad);
pz = pz';


%%
% If we want to plot only the particle in a given energy range
%
Emin = 54;
Emax = 66;
indexForSelection = find(energy < 66 & energy > 54);

theta_1 = acos(pz(indexForSelection));
theta_2 = acos(px(indexForSelection));
theta_3 = acos(py(indexForSelection));

theta_1Deg = radtodeg(theta_1);
theta_2Deg = radtodeg(theta_2);
theta_3Deg = radtodeg(theta_3);

energy = energy(indexForSelection); 

%%
% Scrivo il file buono per Geant4
%
header = {'pz', 'py', 'px', 'z', 'y', 'x', 'ekin', 'pdg', 't'}'; 
M = [pz, py, px, z, y, x, energy, pdg, time];
csvwrite_with_headers('data_p_SelectedInEnergy_30MeV_px_0_06.csv', M, header);
%%
%
figure
subplot(3,1,1)
hist(theta_1Deg, 100)
xlabel('Deg')
title('Theta 1 (pz)')

subplot(3,1,2)
hist(theta_2Deg, 100)
xlabel('Deg')
title('Theta 2 (px)')

subplot(3,1,3)
hist(theta_3Deg, 100)
xlabel('Deg')
title('Theta 3 (py)')

figure
subplot(3,1,1)
hist(pz, 100);
title('pz')

subplot(3,1,2)
hist(py, 100);
title('py')

subplot(3,1,3)
hist(px, 100);
title('px')

figure
title('Angle-energy distributions')
subplot(3,1,1)
plot(theta_1Deg, energy,'.')
xlabel('\theta_1')
ylabel('energy [ MeV ]')

subplot(3,1,2)
plot(theta_2Deg, energy,'.')
xlabel('\theta_2 [ deg ]')
ylabel('energy [ MeV ]')

subplot(3,1,3)
plot(theta_3Deg, energy,'.')
xlabel('\theta_3 [ deg ]')
ylabel('energy [ MeV ]')