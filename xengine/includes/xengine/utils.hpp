#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

namespace XEngine {
	class Utils {
	public:
		/// <summary>
		/// Read all text from file.
		/// </summary>
		/// <param name="t_path">Path to file.</param>
		/// <returns>All text from file.</returns>
		static std::string read_from_file(std::string t_path);
		/// <summary>
		/// Write to file.
		/// </summary>
		/// <param name="t_path">Path to file.</param>
		/// <param name="t_content">All text.</param>
		/// <returns>Sucess of writing.</returns>
		static bool write_file(std::string t_path, std::string t_content);
		/// <summary>
		/// Split string by char.
		/// </summary>
		/// <param name="t_str">Base string.</param>
		/// <param name="t_splitter">Char cplitter.</param>
		/// <returns>Vector of strings.</returns>
		static std::vector<std::string> split_str(std::string* t_str, char t_splitter);
	};
}

#endif // UTILS_H