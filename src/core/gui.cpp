#include <core/gui.hpp>

uppexo::Gui::Gui(GuiBlueprint guiBlueprint) {
  uppexo::Log::GetInstance().logInfo("Initializing ImGUI subsystem\n");

  RenderpassBlueprint renderpassBlueprint(*guiBlueprint.device);
  auto swapchainAttachment = renderpassBlueprint.addAttachment(
      uppexo::presetAttachmentBlueprint::SwapchainAttachment());
  auto depthAttachment = renderpassBlueprint.addAttachment(
      uppexo::presetAttachmentBlueprint::DepthAttachment());
  renderpassBlueprint.attachment[swapchainAttachment].imageLoad =
      VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  renderpassBlueprint.attachment[depthAttachment].imageLoad =
      VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  renderpassBlueprint.addSubpass({swapchainAttachment}, depthAttachment);
  renderpass = std::make_shared<Renderpass>(renderpassBlueprint);

  VkDescriptorPoolSize pool_sizes[] = {
      {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1},
  };
  VkDescriptorPoolCreateInfo pool_info = {};
  pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
  pool_info.maxSets = 1;
  pool_info.poolSizeCount = (uint32_t)1;
  pool_info.pPoolSizes = pool_sizes;
  if (vkCreateDescriptorPool(guiBlueprint.device->getLogicalDevice(),
                             &pool_info, nullptr,
                             &descriptorPool) != VK_SUCCESS) {
    uppexo::Log::GetInstance().logError("Failed to create descriptor pool!\n");
  }

  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForVulkan(guiBlueprint.instance->getWindow(), true);
  ImGui_ImplVulkan_InitInfo init_info = {};
  init_info.Instance = guiBlueprint.instance->getInstance();
  init_info.PhysicalDevice = guiBlueprint.device->getPhysicalDevice();
  init_info.Device = guiBlueprint.device->getLogicalDevice();
  init_info.QueueFamily =
      guiBlueprint.device->getQueue(QueueType::graphic).queueFamilyID.value();
  init_info.Queue = guiBlueprint.device->getQueue(QueueType::graphic).queue[0];
  init_info.PipelineCache = pipelineCache;
  init_info.DescriptorPool = descriptorPool;
  init_info.Subpass = 0;
  init_info.MinImageCount = 2;
  init_info.ImageCount = 2;
  init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
  ImGui_ImplVulkan_Init(&init_info, renderpass->getRenderPass());

  uppexo::Log::GetInstance().logVerbose("Uploading ImGUI font texture\n");
  auto singleUseCommandBuffer =
      guiBlueprint.commandbuffer->createSingleUseCommandBuffer();
  ImGui_ImplVulkan_CreateFontsTexture(singleUseCommandBuffer);
  guiBlueprint.commandbuffer->submitSingleUseCommandBuffer(
      singleUseCommandBuffer);

  vkDeviceWaitIdle(guiBlueprint.device->getLogicalDevice());
  ImGui_ImplVulkan_DestroyFontUploadObjects();

  this->device = guiBlueprint.device->getLogicalDevice();
}

uppexo::Gui::~Gui() {
  uppexo::Log::GetInstance().logInfo("Deinitializing ImGUI subsystem\n");

  renderpass.reset();
  vkDestroyDescriptorPool(device, descriptorPool, nullptr);

  ImGui_ImplVulkan_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void uppexo::Gui::render() {
  ImGui_ImplVulkan_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  ImGui::Begin("Hello, world!");
  ImGui::Text("This is some useful text.");
  ImGui::End();
  ImGui::Render();
}
