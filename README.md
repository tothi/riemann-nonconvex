# riemann-nonconvex
Simulation of a totally asymmetric attractive interacting particle system

## Microscopic model
Markov jump process on a 1-dim discrete N-torus with rates c between neighbouring lattice points.

### Initial data
Step-function (Riemann problem)

## Files
* `runsim.sh`: script to run a bunch of simulations and create movies
* `sim/sim.c`: source code of the simulation

View the comments in `sim.c` to adjust parameters. Feel free to
customize `runsim.sh` to run the simulations needed.

## Example
Here are some result videos from example runs using common parameters
and different initial densities.

The common paramaters are:
* number of lattice points on torus: `N=5000`
* parameters in rates: `A=0`, `B=0.0176654`
* time elapsed between shots taken: `DT_SHOT=5000`
* total number of shots: `SHOTS=5000`
* number of independent simulations to calc the average: `SUM=10000`

The following examples illustrate the different phenomenons due to the initial densities on the left (`vl`) and right (`vr`):

vl   |vr   |center|edge|video link
-----|-----|------|----|----------
 0.7 | 0.5 | R    | S  |<a href="https://www.youtube.com/watch?v=ZJaw3-kXIlE" target="_blank"><img src="https://img.youtube.com/vi/ZJaw3-kXIlE/3.jpg"/></a>
 0.8 |-0.65| RSR  | S  |<a href="https://www.youtube.com/watch?v=Nf5TXqtdhdg" target="_blank"><img src="https://img.youtube.com/vi/Nf5TXqtdhdg/3.jpg"/></a>
-0.74|-0.4 | S    | R  |<a href="https://www.youtube.com/watch?v=sHPGbf8d5tI" target="_blank"><img src="https://img.youtube.com/vi/sHPGbf8d5tI/3.jpg"/></a>
 0.15|-0.55| SR   | SR |<a href="https://www.youtube.com/watch?v=VIF5jHwUFG8" target="_blank"><img src="https://img.youtube.com/vi/VIF5jHwUFG8/3.jpg"/></a>
-0.4 | 0.5 | SRS  | RS |<a href="https://www.youtube.com/watch?v=OSIPnizKTfU" target="_blank"><img src="https://img.youtube.com/vi/OSIPnizKTfU/3.jpg"/></a>
 0.75| 0   | RS   | S  |<a href="https://www.youtube.com/watch?v=63aZPZKR1_4" target="_blank"><img src="https://img.youtube.com/vi/63aZPZKR1_4/3.jpg"/></a>

(abbrevations: S=shock wave, R=rarefaction fan)
