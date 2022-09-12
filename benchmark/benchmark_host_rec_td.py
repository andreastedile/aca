import os

from runexpy.campaign import Campaign
from runexpy.runner import SimpleRunner

IMG_DIR = "/home/gabriele.masina/img_bench/"
IMAGES = [f"{IMG_DIR}{img}" for img in os.listdir(IMG_DIR)]
N_RUNS = 10


def main():
    script = [
        "qsub",
        "-q",
        "short_gpuQ",
        "-l",
        "select=1:ncpus=16:mem=10gb:gpu_type=V100,walltime=00:01:00",
        "--",
        "/home/gabriele.masina/aca/benchmark/pbs_bash",
        "/home/gabriele.masina/aca/build/apps/recursive/qt-rec-td",
    ]
    output_dir = "results/host"
    default_params = {
        "input": None,
        "no-output-file": False,
        "csv": False,
    }
    RUNS = {"input": IMAGES, "no-output-file": True, "csv": True}
    campaign = Campaign.new(script, output_dir, default_params, overwrite=False)
    runner = SimpleRunner(delay=10)
    campaign.run_missing_experiments(runner, RUNS, count=N_RUNS)


if __name__ == "__main__":
    main()
