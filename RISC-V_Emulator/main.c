#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "cpu.h"
#include "inst_defs.h"

#include "assembler.h"

int main(const int argc, const char* argv[])
{
	if (argc < 3 || argc > 4)
	{
		printf("Usage: compile source destination \n");
		printf("Usage: run source \n");
		return -1;
	}

	if (strcmp(argv[1], "compile") == 0)
	{
		const char* file_open_path = argv[2];
		const char* file_save_path = argv[3];

		FILE* file_open_pointer;
		errno_t file_open_error = fopen_s(&file_open_pointer, file_open_path, "rb");
		if (file_open_error)
		{
			char error_msg[255] = { "\0" };
			strerror_s(error_msg, 255, file_open_error);
			printf("ERROR: Cannot read file %s %s\n", file_open_path, error_msg);
			return -1;
		}

		fseek(file_open_pointer, 0, SEEK_END);
		size_t file_open_size = (size_t)ftell(file_open_pointer);
		fseek(file_open_pointer, 0, SEEK_SET);

		char* file_open_string = (char*)calloc(file_open_size + 1, sizeof(char));
		myassert(fread(file_open_string, 1, file_open_size, file_open_pointer) != file_open_size)

		uint32_t compiled_insts_count = 0;
		uint32_t* compiled_insts = assemble(file_open_string, file_open_size, &compiled_insts_count);

		free(file_open_string);
		fclose(file_open_pointer);

		FILE* file_save_pointer;
		errno_t file_save_error = fopen_s(&file_save_pointer, file_save_path, "wb");
		if (file_save_error)
		{
			char error_msg[255] = { '\0' };
			strerror_s(error_msg, 255, file_save_error);
			printf("ERROR: Cannot write file %s %s\n", file_save_path, error_msg);
			return -1;
		}

		myassert(fwrite(compiled_insts, 4, compiled_insts_count, file_save_pointer) != compiled_insts_count)

		free(compiled_insts);
		fclose(file_save_pointer);
	}
	else if (strcmp(argv[1], "run") == 0)
	{
		const char* file_open_path = argv[2];

		FILE* file_open_pointer;
		errno_t file_open_error = fopen_s(&file_open_pointer, file_open_path, "rb");
		if (file_open_error)
		{
			char error_msg[255] = { "\0" };
			strerror_s(error_msg, 255, file_open_error);
			printf("ERROR: Cannot read file %s %s\n", file_open_path, error_msg);
			return -1;
		}

		fseek(file_open_pointer, 0, SEEK_END);
		size_t file_open_size = (size_t)ftell(file_open_pointer);
		fseek(file_open_pointer, 0, SEEK_SET);

		char* file_open_compiled = (char*)calloc(file_open_size, sizeof(char));
		myassert(fread(file_open_compiled, 1, file_open_size, file_open_pointer) != file_open_size);

		riscv_cpu cpu = create_riscv_cpu();

		load_from_uint8(&cpu, file_open_compiled, file_open_size);

		run_riscv_cpu(&cpu);

		destroy_riscv_cpu(&cpu);

		free(file_open_compiled);
		fclose(file_open_pointer);
	}
	else
	{
		printf("Usage: compile source destination \n");
		printf("Usage: run source \n");
		return -1;
	}

	return 0;
}