import os

from runexpy.campaign import Campaign
from runexpy.runner import SimpleRunner
from util import get_size

IMG_DIR = "/home/gabriele.masina/img_bench/"
IMAGES = [f"{IMG_DIR}{img}" for img in os.listdir(IMG_DIR)]
N_RUNS = 10


def main():
    script = [
        "qsub",
        "-q",
        "short_gpuQ",
        "-l",
        "select=1:ncpus=8:mem=10gb:gpu_type=V100,walltime=00:01:00",
        "--",
        "/home/gabriele.masina/aca/benchmark/pbs_bash",
        "/home/gabriele.masina/aca/build/apps/iterative/qt-it-gpu",
    ]
    output_dir = "results/device"
    default_params = {
        "input": None,
        "no-output-file": False,
        "blocks": 1024,
        "threads": 1024,
        "csv": False,
    }
    RUNS = []
    for image in IMAGES:
        # read ppm img shape
        npixels = get_size(image)
        for block_exp in range(8):
            blocks = 4**block_exp
            for thread_exp in range(5):
                threads = 4**thread_exp
                if npixels // 16 <= blocks * threads <= npixels:
                    RUNS.append(
                        {
                            "input": image,
                            "blocks": blocks,
                            "threads": threads,
                            "no-output-file": True,
                            "csv": True,
                        }
                    )
    campaign = Campaign.new(script, output_dir, default_params, overwrite=False)
    runner = SimpleRunner(delay=10)
    campaign.run_missing_experiments(runner, RUNS, count=N_RUNS)


if __name__ == "__main__":
    main()
