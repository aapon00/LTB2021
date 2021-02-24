#include "Size4096.h"

void Size4096::GenerateTemplateFieldTypes() {
	myTemplate["data"] = new CharDataPoint("char", CharacterType::alphanumeric, 4096);
}
