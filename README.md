# comp-neuro
Contains code for spinal dorsal horn modeling - summer 2026 research


1. Install python3 (from https://www.python.org/downloads/)

Then open the terminal (for Macs) and run the following commands:
2. Install NEURON and NetPyNE:
>> pip3 install neuron
>> pip3 install netpyne

3. Change to the directory containing SDH model files.
Example:
>> cd ~/Downloads/SDHmodel

4. Compile the mod files:
>> nrnivmodl mods

5. Run the script to initialize the model:
>> nrniv -python init_mechanical.py

Running the model scripts (Step 5) can also be done in the interative Python (IPython) shell:
>> ipython
>> run init_mechanical.py