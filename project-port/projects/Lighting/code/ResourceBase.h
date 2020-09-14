
#include <string>
#include <vector>

namespace ResourceLib {
	/** Resource ID class. */
	class ResourceID {
	public:
		/** The actual id of the resource. */
		unsigned long id;

		/** Remote list index handled by rendering engine. */
		size_t index;

		/** Constructor that increments resource counter. */
		//ResourceID() {
		//	id = counter++;
		//}

		/*ResourceID& operator = (const ResourceID& other) {
			this->id = other.id;
			return *this;
		}*/

		bool operator == (const ResourceID& other) {
			return this->id == other.id;
		}

		bool operator != (const ResourceID& other) {
			return this->id != other.id;
		}
	private:
		/** The counter which should avoid conflicts. */
		//static unsigned long counter;
	};
}

//unsigned long ResourceLib::ResourceID::counter = 1;