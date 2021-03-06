# fft.hpp

Public-domain single-header library implementing radix-2 decimation-in-time FFT (i.e. FFT for powers of 2)

This software is dual-licensed to the public domain and under the following
license: you are granted a perpetual, irrevocable license to copy, modify,
publish, and distribute this file as you see fit.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
THIS SOFTWARE.

## fft_core

(in_real[], in_imag[], size, gap, out_real[], out_imag[], forwards)

* in_real:    pointer to real-valued spatial samples (for audio, this is where your entire audio signal goes)
* in_imag:    pointer to imaginary-valued ones (not useful for audio)
    *     in_imag is allowed to be nullptr. If so, it will be treated as if it were all zeroes.
* size:       number of complex samples per domain. for audio, this is the number of real samples you have. must be a power of 2. Algorithm will definitely fail and possibly crash otherwise, not tested.
* gap:        must be 1 for outside callers. used for recursion.
* out_real:   pointer to space for real-valued output. does not need to be initialized. must be allocated.
* out_imag:   same as above, for imaginary. not optional.
    *     out_real and out_imag work together to store a complex number (2d vector) representing the phase and amplitude of the given frequency band, even for wholly real inputs.
* forwards:   if true, transform is forwards (fft). if false, transform is backwards (ifft).

For a 8-sample input, the FFT's last three bins contain "negative" frequencies. (So, the last (size/2)-1 bins.) They are only meaningful for complex inputs.

Only does the hard part, which means that the output will be louder if you keep applying this recursively.

## fft

(\<same as fft_core, sans [gap] and [forwards]>)

* compute forwards fft, normalized by 1/sqrt(size)

## ifft

(\<same as fft_core, sans [gap] and [forwards]>)

* above (including normalization) but inverse fft

## normalize_fft

(in_real[], in_imag[], size)

* divide the amplitude of each bin by the number of bins. modifies the input. only needed if you're using fft_core.

## half_normalize_fft

(in_real[], in_imag[], size)

* above, but uses the square root of the number of bins. done by default for fft() and ifft(), only needed if you're using fft_core.

You may want to normalize by sqrt(size), but if so, that's up to you. Just copy the code or make your own.

## sanitize_fft

(in_real[], in_imag[], size)

* moves all data to positive-frequency bins. yes, FFTs have negative frequencies for some reason. they're used to retain correlation data for complex inputs. for real inputs, the negative frequencies just mirror the positive ones and sap half their amplitude, therefore this function. for an explanation of what negative frequencies mean, see http://dsp.stackexchange.com/questions/431/what-is-the-physical-significance-of-negative-frequencies .

## unsanitize_fft

(in_real[], in_imag[], size)

* undo the above. note again that these two fuctions are not sensical for complex inputs.
