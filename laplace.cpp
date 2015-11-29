#include <vector>
#include <unordered_map>
#include <string>

extern "C" {
	#include <Python.h>
}

typedef std::vector<std::wstring>  Corpus;
typedef std::unordered_map<std::wstring, std::unordered_map<wchar_t, double>> Freqs;


static std::vector<std::wstring> pycorpus_to_cxxcorpus(PyObject* py_text_corpus) {
	std::vector<std::wstring> result;
	size_t size = PyObject_Length(py_text_corpus);
	for (size_t index = 0; index < size; ++index) {
		result.push_back(std::wstring(PyUnicode_AsUnicode(PyList_GetItem(py_text_corpus, index))));
	}
	return result;
}

static Freqs calculate_ngram_frequencies(const Corpus& corpus, size_t n_value, double alpha) {
	Freqs result;
	std::wstring prefix = L"";
	for (size_t i = 0; i + 1 < n_value; ++i) {
		prefix += L'$';
	}

	for (const auto& text : corpus) {
		std::wstring ntext = prefix + text;
		for (size_t pos = 0; pos + n_value <= ntext.length(); ++pos) {
			result[ntext.substr(pos, n_value - 1)][ntext[pos + n_value - 1]] += 1.;
		}
	}

	for (auto& pair : result) {
		double norm = alpha * pair.second.size();
		for (const auto& transition : pair.second) {
			norm += transition.second;
		}
		for (auto& transition : pair.second) {
			transition.second = (transition.second + alpha) / norm;
		}
	}

	return result;
}


static PyObject* cxx_to_pyobject(const Freqs& frequences) {
	PyObject* result = PyDict_New();
	for (const auto& pair : frequences) {
		PyObject* item = PyDict_New();
		for (const auto& transition : pair.second) {
			PyDict_SetItem(item, PyUnicode_FromUnicode(&transition.first, 1), PyFloat_FromDouble(transition.second));
		}
		PyDict_SetItem(result, PyUnicode_FromUnicode(pair.first.c_str(), pair.first.length()), item);
	}
	return result;
}


static PyObject* build_laplace_frequences(PyObject * module, PyObject * args) {
	const auto corpus = pycorpus_to_cxxcorpus(PyTuple_GetItem(args, 0));
	const auto n_value = PyLong_AsSsize_t(PyTuple_GetItem(args, 1));
	const auto alpha = PyFloat_AsDouble(PyTuple_GetItem(args, 2));

	const auto frequences = calculate_ngram_frequencies(corpus, n_value, alpha);

	PyObject * py_result = cxx_to_pyobject(frequences);
	return py_result;
}

PyMODINIT_FUNC PyInit_laplace()
{
	static PyMethodDef ModuleMethods[] = {
		{ "build_laplace_frequences", build_laplace_frequences, METH_VARARGS, "build_laplace_frequences(corpus, n_value, alpha).\n\tcorpus -\
		list of strings\n\tn_value - \
		frequencies for ngrams will be built\n\talpha - \
		smoothing parameter.\n\n\tDesciption: Building Laplace frequences\n"},
		{ NULL, NULL, 0, NULL }
	};
	static PyModuleDef ModuleDef = {
		PyModuleDef_HEAD_INIT,
		"laplace",
		"Documentation.\nThis module contains function build_laplace_frequences. It builds dictionary of ngrams' frequences using Laplace smothing.",
		-1, ModuleMethods, 
		NULL, NULL, NULL, NULL
	};
	PyObject * module = PyModule_Create(&ModuleDef);
	return module;
}
