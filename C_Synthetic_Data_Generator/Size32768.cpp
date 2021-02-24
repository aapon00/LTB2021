#include "Size32768.h"

void Size32768::GenerateTemplateFieldTypes() {
	myTemplate["data"] = new CharDataPoint("char", CharacterType::alphanumeric, 32768);
}
