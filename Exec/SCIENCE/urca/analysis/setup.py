from setuptools import setup, find_packages

setup(name='UrcaAnalysis',
      version='0.1',
      description='Urca analysis scripts',
      url='git@github.com:AMReX-Astro/MAESTROex.git',
      author='Donald E. Willcox',
      author_email='eugene.willcox@gmail.com',
      license='BSD',
      packages=find_packages(),
      package_data={"UrcaAnalysis": ["UrcaAnalysis/*"]},
      scripts=['scripts/ffmpeg_make_mp4',
               'scripts/lineout-field',
               'scripts/lineout-field-testing',
               'scripts/mk_gradpi_slices',
               'scripts/plot-diags',
               'scripts/plot-edot',
               'scripts/plot_fconv_slopes',
               'scripts/plot-neutrino-spectrum',
               'scripts/power-spectrum',
               'scripts/profile-field',
               'scripts/slice-convgrad',
               'scripts/slice-enucdot',
               'scripts/slice-field',
               'scripts/slice-vel',
               'scripts/sort-strings',
               'scripts/streamlines',
               'scripts/sum-field',
               'scripts/track-enucdot-profile',
               'scripts/urca23-quick-post',
               'scripts/urca-quick-post',
               'scripts/volume-plot-circum_vel',
               'scripts/volume-plot-enucdot',
               'scripts/volume-plot-field',
               'scripts/volume-plot-rad_vel',
               'scripts/volume-plot-testing',
               'scripts/volume-plot-ye'],
      install_requires=['numpy', 'matplotlib', 'yt'],
      zip_safe=False)
