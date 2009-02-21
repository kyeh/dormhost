class CreateRoomImages < ActiveRecord::Migration
  def self.up
    create_table :room_images do |t|
      t.column  :room_id, :integer, :null => false
      t.column  :image,   :string

      t.timestamps
    end
  end

  def self.down
    drop_table :room_images
  end
end
