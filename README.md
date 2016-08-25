# riemann-nonconvex
Simulation of a totally asymmetric attractive interacting particle system

This is a supplemental material to the article
*Márton Balázs, Attila László Nagy, Bálint Tóth, István Tóth:*
**Coexistence of Shocks and Rarefaction Fans: Complex Phase Diagram of a Simple Hyperbolic Particle System** *(2016)*.
The article was published in the
[Journal of Statistical Physics](http://link.springer.com/journal/10955) (now in the
[Online First section](http://link.springer.com/journal/10955/onlineFirst/page/1)).
The official abstract page is available [here](http://link.springer.com/article/10.1007/s10955-016-1600-2).
A read-only shared PDF version of the full text can be found
[here](http://www.readcube.com/articles/10.1007/s10955-016-1600-2?author_access_token=GTB7cm6HV9zoJWP1_rN6NPe4RwlQNchNByi7wbcMAY4FizNqvrhqK12KP10_76IpPS8GvIEQ7Hn9abtNv7NNggfL_5rwk2MrQ570e0UYAyjwWxwHzpvsU47IqySZzLG4_VNVIl3a-7sdFPcOaHjYKA%3D%3D)


## Microscopic model
Markov jump process on a 1-dim discrete N-torus with rates c between neighbouring lattice points.

### Initial data
Step-function (Riemann problem)

## Files
* `runsim.sh`: script to run a bunch of simulations and create movies
* `sim/sim.c`: source code of the simulation

View the comments in `sim.c` to adjust parameters. Feel free to
customize `runsim.sh` to run the simulations needed.

## Examples
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
