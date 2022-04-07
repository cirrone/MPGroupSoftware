from SDFfileFormat import *
import pathlib
import cv2
from cv2 import VideoWriter, VideoWriter_fourcc
import numpy as np


def SDF_movie(outputfolder, SimulationName, initialfile, finalfile, quantity):
    h = cv2.imread(outputfolder + f'{initialfile:04d}' + quantity + '.png')
    height = h.shape[0]
    width = h.shape[1]
    FPS = 1.0
    fourcc = VideoWriter_fourcc(*'mp4v')
    video = VideoWriter(outputfolder + SimulationName + quantity + '.mp4', fourcc, float(FPS), (width, height))
    for filenumber in range(initialfile, finalfile):
        sdffilenumber = formatfilenumber(filenumber, formatzeros=3)
        filename = outputfolder + sdffilenumber + quantity + '.png'
        filepath = pathlib.Path(filename)
        if filepath.exists():
            h = cv2.imread(filename)
            video.write(np.uint8(h))
        else:
             print(sdffilenumber + 'image does not exist')
    video.release()
