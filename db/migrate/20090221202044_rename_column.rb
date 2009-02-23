class RenameColumn < ActiveRecord::Migration
  def self.up
    rename_column :room_images,  :room_id, :room_profile_id
  end

  def self.down
    rename_column :room_images,  :room_profile_id, :room_id
  end
end
