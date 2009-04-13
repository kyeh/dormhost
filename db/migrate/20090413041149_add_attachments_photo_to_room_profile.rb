class AddAttachmentsPhotoToRoomProfile < ActiveRecord::Migration
  def self.up
    add_column :room_profiles, :photo_file_name, :string
    add_column :room_profiles, :photo_content_type, :string
    add_column :room_profiles, :photo_file_size, :integer
    add_column :room_profiles, :photo_updated_at, :datetime
  end

  def self.down
    remove_column :room_profiles, :photo_file_name
    remove_column :room_profiles, :photo_content_type
    remove_column :room_profiles, :photo_file_size
    remove_column :room_profiles, :photo_updated_at
  end
end
