#include "G4GDMLSetup.hh"

G4VPhysicalVolume *G4GDMLSetup::Get(const std::string &name) {

   if (setupMap.find(name) == setupMap.end()) {
   
      std::cout << std::endl;
      std::cout << "G4GDML ERROR! setup '" << name << "' can not be found!" << std::endl;
      std::cout << std::endl;
      return 0;
   }

   std::string worldref = setupMap[name];

   G4LogicalVolume *volume = G4LogicalVolumeStore::GetInstance()->GetVolume(worldref,false);

   if (volume == 0) {
   
      std::cout << std::endl;
      std::cout << "G4GDML ERROR! volume '" << worldref << "' referenced in setup '" << name << "' was not found!" << std::endl;
      std::cout << std::endl;
      return 0;
   }

   volume->SetVisAttributes(G4VisAttributes::Invisible);

   return new G4PVPlacement(0,G4ThreeVector(),volume,name,0,0,0);
}

bool G4GDMLSetup::Read(const xercesc::DOMElement* const element) {

   std::string name("");

   XMLCh *name_attr = xercesc::XMLString::transcode("name");

   name = xercesc::XMLString::transcode(element->getAttribute(name_attr));

   xercesc::XMLString::release(&name_attr);

   const xercesc::DOMNodeList* const children = element->getChildNodes();
   const  XMLSize_t nodeCount = children->getLength();

   for (XMLSize_t i=0;i<nodeCount;i++) {

      xercesc::DOMNode* node = children->item(i);
      
      if (node->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;
   
      const xercesc::DOMElement* const child = dynamic_cast<xercesc::DOMElement*>(node);   

      const std::string tag = xercesc::XMLString::transcode(child->getTagName());

      if (tag != "world") return false;

      XMLCh *ref_attr = xercesc::XMLString::transcode("ref");
      std::string ref = xercesc::XMLString::transcode(child->getAttribute(ref_attr));
      xercesc::XMLString::release(&ref_attr);

      setupMap[name] = ref;

      return true;
   }

   return false;
}
