import os
import numpy as np
import matplotlib.pyplot as plt
from dune import DuneSediment
from tqdm.auto import tqdm


def get_normalized_height(dune: DuneSediment) -> np.ndarray:
    height = (dune.bedrock + dune.sediments).copy()

    min = height.min()
    max = height.max()

    height = (height - min) / (max - min)
    return height


def main():
    os.makedirs("results", exist_ok=True)

    # transverse dunes
    dune = DuneSediment(256, 256, r_min=3, r_max=5, wind=(3, 0))
    for _ in tqdm(range(300), desc="Transverse Dunes"):
        dune.step()

    height = get_normalized_height(dune)
    plt.imshow(height)
    plt.savefig(os.path.join("results", "transverse.png"))

    # barchan dunes
    dune = DuneSediment(256, 256, r_min=0.5, r_max=2.0, wind=(5, 0))
    for _ in tqdm(range(300), desc="Barchan Dunes"):
        dune.step()

    height = get_normalized_height(dune)
    plt.imshow(height)
    plt.savefig(os.path.join("results", "barchan.png"))

    # yardangs dunes
    dune = DuneSediment(256, 256, r_min=0.5, r_max=0.5, wind=(6, 0), abrasion_on=True)
    for _ in tqdm(range(600), desc="Yardangs"):
        dune.step()

    height = get_normalized_height(dune)
    plt.imshow(height)
    plt.savefig(os.path.join("results", "yardangs.png"))

    # nabkha dunes
    dune = DuneSediment(256, 256, r_min=3, r_max=5, wind=(3, 0), vegetation_on=True)
    for _ in tqdm(range(300), desc="Nabkha"):
        dune.step()

    height = get_normalized_height(dune)
    plt.imshow(height)
    plt.savefig(os.path.join("results", "nabkha.png"))


if __name__ == "__main__":
    main()
