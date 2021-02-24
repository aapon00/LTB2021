#include "Size8192.h"

void Size8192::GenerateTemplateFieldTypes() {
	myTemplate["data"] = new CharDataPoint("char", CharacterType::alphanumeric, 8192);
}
