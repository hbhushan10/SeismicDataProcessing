# Denoise Filter for Seismic Data Processing

## Overview
Seismic data often contains various types of noise that degrade the quality of seismic images and affect further processing such as migration and inversion.  
This project implements a **denoise filter** to reduce noise from seismic traces while preserving the important signal components.

The filter can be applied to **seismic traces,  or seismic images** to improve signal clarity and stability in downstream processing workflows such as **Reverse Time Migration (RTM)** and **Full Waveform Inversion (FWI)**.

---

## Features
- Simple and efficient denoise filtering
- Works on **floating point seismic data**
- Supports **1D seismic trace filtering**
- Reduces random high-frequency noise
- Easy integration with existing seismic workflows
- Implemented in **C/C++ and openMP (Multithreading)**

---

## Algorithm

The denoise filter smooths the data by averaging neighboring samples.  
This reduces random noise while maintaining the overall waveform structure.

Basic formula:

\[
output[i] = \frac{input[i-1] + input[i] + input[i+1]}{3}
\]

Where:
- `input[i]` = seismic amplitude at sample `i`
- `output[i]` = filtered amplitude

---

## Project Structure
