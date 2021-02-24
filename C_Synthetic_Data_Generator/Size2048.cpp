#include "Size2048.h"

void Size2048::GenerateTemplateFieldTypes() {
	myTemplate["data"] = new CharDataPoint("char", CharacterType::alphanumeric, 2048);
}
