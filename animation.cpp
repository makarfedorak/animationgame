#include <COLLADAFramework/COLLADAFW.h>

std::vector<Animation> animations;

bool loadAnimations(const std::string& filename) {
    COLLADAFW::Loader loader;
    COLLADAFW::DocumentImporter* importer = loader.createDocumentImporter();
    if (!importer->loadFile(filename)) {
        return false;
    }
    const COLLADAFW::AnimationList& animationList = importer->getAnimationList();
    for (unsigned int i = 0; i < animationList.getCount(); i++) {
        animations.push_back(animationList[i]);
    }
    return true;
}

std::vector<Bone> bones;

void updateAnimation(double dt) {
    for (unsigned int i = 0; i < animations.size(); i++) {
        Animation animation = animations[i];
        double weight = animation.getWeight();
        for (unsigned int j = 0; j < animation.getNumChannels(); j++) {
            const AnimationChannel& channel = animation.getChannel(j);
            Bone& bone = bones[channel.getTargetNode()];
            bone.transform = blend(bone.transform, channel.getTransform(), weight);
        }
    }
}

std::vector<Bone> bones;

void updateSkeleton() {
    for (unsigned int i = 0; i < bones.size(); i++) {
        Bone& bone = bones[i];
        if (bone.hasParent()) {
            Bone& parent = bones[bone.getParent()];
            bone.globalTransform = parent.globalTransform * bone.localTransform;
        } else {
            bone.globalTransform = bone.localTransform;
        }
    }
}
